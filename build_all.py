import sys
import subprocess
import os
import argparse
import json
import time
from datetime import datetime
from concurrent.futures import ThreadPoolExecutor, as_completed

_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# --publish target: the rolling release the docs link to.
PUBLISH_REPO = 'holykeebs/qmk_compiled'
PUBLISH_TAG = 'latest'
# The branch a publish must be built from, in both this repo and the overlay.
PUBLISH_BRANCH = 'hk-master'
# Release assets built outside this matrix (from separate forks) and uploaded by
# hand. The prune step leaves these alone instead of deleting them.
PRESERVED_ASSETS = {
    'tarohayashi_killerwhale_duo_default.uf2',
}
# Uploading hundreds of assets in one gh invocation works but reports nothing
# until the end; chunking gives progress and retry granularity.
UPLOAD_CHUNK = 50

class Command:
    def __init__(self, kb, km, repo=None) -> None:
        self.kb = kb
        self.km = km
        # Checkout to run `make` in; None builds in this repo. The vial fork's
        # keyball61plus builds set this to the vial-qmk checkout.
        self.repo = repo
        self.arguments = []
        self.left_pointing_device = None
        self.right_pointing_device = None
        self.oled = None

    def prepend_argument(self, argument):
        self.arguments.insert(0, argument)
        self.arguments.insert(0, '-e')

    def set_pointing(self, kind, side):
        if side == 'left':
            self.left_pointing_device = kind
        elif side == 'right':
            self.right_pointing_device = kind
        else:
            raise ValueError('Invalid side: {}'.format(side))

    def add_argument(self, argument):
        self.arguments.append('-e')
        self.arguments.append(argument)

    def add_argument_raw(self, argument):
        self.arguments.append(argument)

    def arguments_list(self):
        a = list(self.arguments)
        if self.oled:
            a.append('-e')
            a.append(f'OLED={self.oled}')
        return a

    def build(self):
        return ' '.join(self.build_list())

    def build_list(self):
        l = ['make', f'{self.kb}:{self.km}'] + self.arguments_list()
        return l

    def file_name(self):
        parts = []
        parts.append(self.kb.replace('/', '_'))
        parts.append(self.km)

        if not self.kb.startswith('keyball'):
            if not self.left_pointing_device and not self.right_pointing_device:
                if self.oled:
                    parts.append('oled')
                else:
                    parts.append('none')
            else:
                if self.left_pointing_device:
                    parts.append(self.left_pointing_device)
                elif self.oled:
                    parts.append('oled')
                else:
                    parts.append('none')

                if self.right_pointing_device:
                    parts.append(self.right_pointing_device)
                elif self.oled:
                    parts.append('oled')
                else:
                    parts.append('none')

        console = False
        for argument in self.arguments:
            if argument.startswith('SIDE='):
                side = argument[len('SIDE='):]
                parts.append('flash_on_' + side)
            elif argument == 'WHEEL_LED=yes':
                parts.append('wheel_led')
            elif argument == 'CONSOLE=yes':
                console = True
        if console:
            parts.insert(0, 'debug')
        return '_'.join(parts)

def build_commands():
    commands = []
    skip_pointing_devices =[('cirque35', 'cirque40'), ('cirque40', 'cirque35')]
    for console_enabled in [True, False]:
        for kb in ['crkbd/rev1', 'holykeebs/spankbd', 'lily58/rev1', 'holykeebs/sweeq']:
            for left_pointing_device in ['trackpoint', 'trackball', 'cirque35', 'cirque40', 'tps43', '']:
                for right_pointing_device in ['trackpoint', 'trackball', 'cirque35', 'cirque40', 'tps43', '']:
                    if (left_pointing_device, right_pointing_device) in skip_pointing_devices:
                        print('Skipping configuration: ', left_pointing_device, right_pointing_device)
                        continue
                    # by default use the via keymap, but if we have a pointing device, pull in the hk keymap with the pointing
                    # device layer
                    keymap = 'via'
                    if left_pointing_device or right_pointing_device:
                        keymap = 'hk'
                    if left_pointing_device and right_pointing_device:
                        for side in ('left', 'right'):
                            command = Command(kb, keymap)
                            if keymap == 'hk' and console_enabled:
                                command.add_argument(f'CONSOLE=yes')
                            command.set_pointing(left_pointing_device, 'left')
                            command.set_pointing(right_pointing_device, 'right')
                            command.add_argument(f'POINTING_DEVICE={left_pointing_device}_{right_pointing_device}')
                            command.add_argument(f'SIDE={side}')
                            if left_pointing_device == 'trackball' or right_pointing_device == 'trackball':
                                command.add_argument(f'TRACKBALL_RGB_RAINBOW=yes')
                            commands.append(command)
                    else:
                        for with_oled in [True, False]:
                            command = Command(kb, keymap)
                            if keymap == 'hk' and console_enabled:
                                command.add_argument(f'CONSOLE=yes')
                            if with_oled:
                                command.oled = 'stock' if keymap == 'via' else 'yes'
                                # flip the OLED if there's a pointing device so the one on the peripheral side shows the
                                # layer and keylogger, otherwise we get the logo
                                if left_pointing_device or right_pointing_device:
                                    command.add_argument(f'OLED_FLIP=yes')
                            if left_pointing_device:
                                command.set_pointing(left_pointing_device, 'left')
                                command.add_argument(f'POINTING_DEVICE={left_pointing_device}')
                                command.add_argument(f'POINTING_DEVICE_POSITION=left')
                                if left_pointing_device == 'trackball':
                                    command.add_argument(f'TRACKBALL_RGB_RAINBOW=yes')
                            elif right_pointing_device:
                                command.set_pointing(right_pointing_device, 'right')
                                command.add_argument(f'POINTING_DEVICE={right_pointing_device}')
                                command.add_argument(f'POINTING_DEVICE_POSITION=right')
                                if right_pointing_device == 'trackball':
                                    command.add_argument(f'TRACKBALL_RGB_RAINBOW=yes')
                            else:
                                pass

                            commands.append(command)

    return commands

class BuildError(Exception):
    def __init__(self, file_name, log_path, message):
        super().__init__(message)
        self.file_name = file_name
        self.log_path = log_path

def destination_for(base_dir, file_name):
    if file_name.startswith('debug'):
        return f'{base_dir}/debug/{file_name}.uf2'
    return f'{base_dir}/{file_name}.uf2'

def finalize_command(command, make_jobs, parallel):
    if parallel > 1:
        # Make each `make` process report its own exit code independently. By
        # default QMK records build failures in a *shared* .build/error_occurred
        # file (it `rm`s it at startup and re-checks it at the end of every
        # invocation), so concurrent make processes race on that file and corrupt
        # each other's exit codes. BREAK_ON_ERRORS makes a failing build `exit 1`
        # directly instead. SKIP_GIT avoids concurrent `git submodule --sync`
        # writes to .git/config at startup.
        command.add_argument('BREAK_ON_ERRORS=yes')
        command.add_argument('SKIP_GIT=yes')
    command.add_argument_raw(f'-j{make_jobs}')
    # A unique TARGET gives each build its own .build/obj_<TARGET> directory and
    # root <TARGET>.uf2, so concurrent builds never clobber each other's output.
    command.prepend_argument(f'TARGET={command.file_name()}')

def run_build(command, base_dir):
    file_name = command.file_name()
    destination = destination_for(base_dir, file_name)
    log_path = f'{base_dir}/logs/{file_name}.log'

    # Archive firmware from a previous run before overwriting it.
    if os.path.exists(destination):
        os.rename(destination, f'{base_dir}/previous/{file_name}.uf2')

    with open(log_path, 'wb') as log:
        log.write(f'{command.build()}\n\n'.encode())
        log.flush()
        proc = subprocess.run(command.build_list(), stdout=log, stderr=subprocess.STDOUT,
                              cwd=command.repo)

    if proc.returncode != 0:
        raise BuildError(file_name, log_path, f'make exited with {proc.returncode}')

    # make drops <TARGET>.uf2 in the root of whichever checkout built it.
    built = os.path.join(command.repo, f'{file_name}.uf2') if command.repo else f'{file_name}.uf2'
    os.rename(built, destination)
    return file_name

def _repo_state(repo_dir):
    """'<branch>  <short-hash>[ (dirty)]' for repo_dir, or None if it can't be
    determined (missing dir, not a git repo, git unavailable). Untracked files
    don't count as dirty."""
    def git(*args):
        return subprocess.run(['git', '-C', repo_dir, *args],
                              capture_output=True, text=True, check=True).stdout.strip()
    try:
        branch = git('rev-parse', '--abbrev-ref', 'HEAD')
        short = git('rev-parse', '--short', 'HEAD')
        dirty = git('status', '--porcelain', '--untracked-files=no')
    except Exception:
        return None
    return f'{branch}  {short}' + (' (dirty)' if dirty else '')

def _overlay_dir():
    """Where the holykeebs userspace overlay lives: QMK_USERSPACE env, else the
    global `qmk config user.overlay_dir`. None if neither is set."""
    env = os.environ.get('QMK_USERSPACE')
    if env:
        return env
    try:
        out = subprocess.run(['qmk', 'config', 'user.overlay_dir'],
                             capture_output=True, text=True, check=True).stdout.strip()
    except Exception:
        return None
    val = out.split('=', 1)[1].strip() if '=' in out else ''
    return val if val and val != 'None' else None

def _vial_dir():
    """Where the vial-qmk fork lives: HK_VIAL_QMK env, else a sibling checkout
    next to this repo. None if neither points at a checkout."""
    candidate = os.environ.get('HK_VIAL_QMK') or os.path.join(os.path.dirname(_SCRIPT_DIR), 'vial-qmk')
    return candidate if os.path.isdir(os.path.join(candidate, 'keyboards')) else None

def commands_preamble():
    """Comment-line preamble recording the source state these builds came from: a
    timestamp and each contributing repo's branch + short hash (+ dirty). A
    holykeebs firmware is compiled from this tree plus the external overlay, so
    both are reported. Repos that can't be resolved are skipped, never fatal."""
    def row(label, value):
        return f'# {(label + ":"):<21}{value}'
    lines = ['# build_all command list',
             row('generated', datetime.now().strftime('%Y-%m-%d %H:%M:%S'))]
    qmk = _repo_state(_SCRIPT_DIR)
    if qmk:
        lines.append(row('qmk_firmware', qmk))
    overlay = _overlay_dir()
    if overlay:
        state = _repo_state(overlay)
        if state:
            lines.append(row('holykeebs-userspace', state))
    vial = _vial_dir()
    if vial:
        state = _repo_state(vial)
        if state:
            lines.append(row('vial-qmk', state))
    lines.append('#')
    return '\n'.join(lines) + '\n'

def _publish_repo_problems(repo_dir, label):
    """Problems that make repo_dir unfit to publish from: wrong branch, dirty
    tracked files, or out of sync with origin. Published artifacts must be
    reproducible from pushed commits, so HEAD has to equal origin/PUBLISH_BRANCH
    exactly (fetched fresh) - not merely share history with it."""
    def git(*args):
        return subprocess.run(['git', '-C', repo_dir, *args],
                              capture_output=True, text=True, check=True).stdout.strip()
    problems = []
    try:
        branch = git('rev-parse', '--abbrev-ref', 'HEAD')
        if branch != PUBLISH_BRANCH:
            problems.append(f'{label}: on branch {branch!r}, publish requires {PUBLISH_BRANCH!r}')
        if git('status', '--porcelain', '--untracked-files=no'):
            problems.append(f'{label}: working tree has uncommitted changes')
        git('fetch', '--quiet', 'origin', PUBLISH_BRANCH)
        local, remote = git('rev-parse', 'HEAD'), git('rev-parse', f'origin/{PUBLISH_BRANCH}')
        if local != remote:
            problems.append(f'{label}: HEAD ({local[:10]}) != origin/{PUBLISH_BRANCH} '
                            f'({remote[:10]}) - push or pull first')
    except Exception as err:
        problems.append(f'{label}: git checks failed in {repo_dir}: {err}')
    return problems

def publish_preflight():
    """All problems that block a publish; empty list means go."""
    problems = _publish_repo_problems(_SCRIPT_DIR, 'qmk_firmware')

    overlay = _overlay_dir()
    if overlay:
        problems += _publish_repo_problems(overlay, 'holykeebs-userspace')
    else:
        problems.append('holykeebs-userspace: overlay not found (QMK_USERSPACE / qmk config user.overlay_dir)')

    vial = _vial_dir()
    if vial:
        problems += _publish_repo_problems(vial, 'vial-qmk')
    else:
        problems.append('vial-qmk: checkout not found (HK_VIAL_QMK or ../vial-qmk)')

    if subprocess.run(['gh', 'auth', 'status'], capture_output=True).returncode != 0:
        problems.append('gh: not authenticated (run `gh auth login`)')
    return problems

def _gh(*args):
    return subprocess.run(['gh', *args, '-R', PUBLISH_REPO],
                          capture_output=True, text=True, check=True).stdout

def release_notes(firmware_count, debug_count):
    state = lambda d: (_repo_state(d) or 'unknown').replace('  ', ' @ ')
    overlay = _overlay_dir()
    vial = _vial_dir()
    return '\n'.join([
        'Precompiled holykeebs firmware for every supported configuration. See the',
        '[firmware docs](https://docs.holykeebs.com/firmware/) for picking a file;',
        '`commands.txt` maps each file to the command that produced it.',
        '',
        f'- generated: {datetime.now().strftime("%Y-%m-%d %H:%M:%S")}',
        f'- qmk_firmware: {state(_SCRIPT_DIR)}',
        f'- holykeebs-userspace: {state(overlay) if overlay else "unknown"}',
        f'- vial-qmk: {state(vial) if vial else "unknown"}',
        f'- firmwares: {firmware_count} ({debug_count} debug)',
    ]) + '\n'

def publish(base_dir, commands, dry_run):
    """Sync the built matrix to the PUBLISH_REPO 'latest' release: upload every
    artifact (clobbering same-named ones), prune remote assets that no longer
    exist locally (except PRESERVED_ASSETS), and refresh the release notes with
    the source provenance."""
    # The upload set is derived from the command matrix, not a directory listing,
    # so stray files in build_all/ can never leak into the release.
    paths = [destination_for(base_dir, c.file_name()) for c in commands]
    paths.append(f'{base_dir}/commands.txt')
    missing = [p for p in paths if not os.path.exists(p)]
    if missing:
        print(f'publish: {len(missing)} artifact(s) missing, e.g. {missing[0]}')
        return 1

    local_names = {os.path.basename(p) for p in paths}
    remote_names = set(json.loads(_gh('release', 'view', PUBLISH_TAG, '--json', 'assets',
                                      '--jq', '[.assets[].name]')))
    prune = sorted(remote_names - local_names - PRESERVED_ASSETS)
    kept = sorted(remote_names & PRESERVED_ASSETS)

    debug_count = sum(1 for n in local_names if n.startswith('debug_'))
    print(f'publish: {len(paths)} assets to upload, {len(prune)} stale remote '
          f'asset(s) to prune, preserving {kept or "none"}')
    if dry_run:
        for name in prune:
            print(f'  would prune: {name}')
        print('publish: dry run, nothing modified')
        return 0

    for name in prune:
        _gh('release', 'delete-asset', PUBLISH_TAG, name, '-y')
        print(f'  pruned {name}')

    start = time.time()
    for i in range(0, len(paths), UPLOAD_CHUNK):
        chunk = paths[i:i + UPLOAD_CHUNK]
        _gh('release', 'upload', PUBLISH_TAG, *chunk, '--clobber')
        print(f'  uploaded {min(i + UPLOAD_CHUNK, len(paths))}/{len(paths)} '
              f'({time.time() - start:.0f}s)')

    _gh('release', 'edit', PUBLISH_TAG,
        '--notes', release_notes(len(paths) - 1, debug_count))
    print(f'publish: done, release notes updated '
          f'(https://github.com/{PUBLISH_REPO}/releases/tag/{PUBLISH_TAG})')
    return 0

def all_commands():
    """The full firmware matrix: dedicated keyball boards (VIA here, Vial from
    the vial-qmk fork), the modular board x pointing-device x OLED matrix, and
    keyball61plus from both forks (this repo and vial-qmk), deduplicated."""
    commands = [
        Command('keyball/keyball39', 'via'),
        Command('keyball/keyball44', 'via'),
        Command('keyball/keyball61', 'via'),
    ]
    for command in build_commands():
        command.prepend_argument('USER_NAME=holykeebs')
        commands.append(command)

    # keyball61plus runs on the holykeebs userspace but isn't part of the modular
    # POINTING_DEVICE matrix above: it's always built dual-PMW3360 combined (the
    # installed ball count is detected at runtime), so add its variants directly.
    # Keyballs ship with the OLED fitted, so every build enables it.
    # Both wheel-LED variants: the default holds the LED under each scroll wheel
    # dark (the wheel body scatters it), WHEEL_LED=yes lights it like any other
    # key, for whoever prefers that.
    for km in ('via', 'default'):
        for wheel_led in (False, True):
            command = Command('holykeebs/keyball61plus', km)
            command.add_argument('USER_NAME=holykeebs')
            if wheel_led:
                command.add_argument('WHEEL_LED=yes')
            command.oled = 'yes'
            commands.append(command)

    # Vial builds live in the vial-qmk fork (Vial needs its older QMK base), so
    # they build inside that checkout and join the matrix here. Skipped with a
    # warning when the checkout is absent; a publish requires it (see
    # publish_preflight). keyball61plus runs on the userspace; keyball39/44/61
    # carry the stock keyball firmware there and take no build vars, their
    # keymap rules.mk already enabling RGBLIGHT and the OLED.
    vial = _vial_dir()
    if vial:
        for wheel_led in (False, True):
            command = Command('holykeebs/keyball61plus', 'vial', repo=vial)
            command.add_argument('USER_NAME=holykeebs')
            if wheel_led:
                command.add_argument('WHEEL_LED=yes')
            command.oled = 'yes'
            commands.append(command)
        for kb in ('keyball/keyball39', 'keyball/keyball44', 'keyball/keyball61'):
            commands.append(Command(kb, 'vial', repo=vial))
    else:
        print('vial-qmk checkout not found (HK_VIAL_QMK or ../vial-qmk); '
              'skipping vial builds')

    # Drop duplicate configurations. The console_enabled loop in build_commands()
    # regenerates every `via` build identically (CONSOLE only affects `hk`), which
    # both wastes a rebuild and breaks the unique-TARGET invariant that keeps
    # concurrent builds from racing on a shared .build/obj_<TARGET> and output.
    seen = set()
    unique = []
    for command in commands:
        name = command.file_name()
        if name not in seen:
            seen.add(name)
            unique.append(command)
    return unique

def main() -> int:
    # Line-buffer stdout so the live progress feed is visible even when this runs
    # in the background with stdout redirected to a file (block-buffered by default).
    if hasattr(sys.stdout, 'reconfigure'):
        sys.stdout.reconfigure(line_buffering=True)

    cpu = os.cpu_count() or 4
    parser = argparse.ArgumentParser(description='Build the holykeebs firmware matrix.')
    parser.add_argument('-p', '--parallel', type=int, default=max(1, cpu // 2),
                        help='number of builds to run concurrently (default: %(default)s)')
    parser.add_argument('-j', '--jobs', type=int, default=0,
                        help='make jobs per build (default: cpu // parallel)')
    parser.add_argument('--rebuild', action='store_true',
                        help='rebuild every firmware, archiving existing outputs to '
                             'previous/. Default skips configs already present in '
                             'build_all/ (resume-friendly); note it does NOT detect '
                             'stale outputs, so pass --rebuild to refresh after source '
                             'changes.')
    parser.add_argument('--publish', action='store_true',
                        help=f'after a full rebuild, sync the matrix to the '
                             f'{PUBLISH_REPO} \'{PUBLISH_TAG}\' release: upload all '
                             f'artifacts, prune stale remote assets (preserving '
                             f'externally-built ones), and refresh the release notes. '
                             f'Requires this repo, the overlay, and the vial-qmk '
                             f'checkout to be on {PUBLISH_BRANCH}, clean, and in '
                             f'sync with origin. Implies --rebuild.')
    parser.add_argument('--dry-run', action='store_true',
                        help='with --publish: build everything and show what would be '
                             'uploaded/pruned without modifying the release.')
    args = parser.parse_args()

    if args.dry_run and not args.publish:
        parser.error('--dry-run only makes sense with --publish')
    if args.publish:
        problems = publish_preflight()
        if problems:
            print('publish preflight failed:')
            for p in problems:
                print(f'  - {p}')
            return 1
        if not args.rebuild:
            # A publish must never ship stale leftovers from an older source state.
            print('publish: forcing --rebuild')
            args.rebuild = True

    parallel = max(1, args.parallel)
    make_jobs = args.jobs if args.jobs > 0 else max(1, cpu // parallel)

    commands = all_commands()

    base_dir = 'build_all'
    for sub in ('debug', 'previous', 'logs'):
        os.makedirs(f'{base_dir}/{sub}', exist_ok=True)

    for command in commands:
        finalize_command(command, make_jobs, parallel)

    with open(f'{base_dir}/commands.txt', 'w') as commands_file:
        commands_file.write(commands_preamble())
        for command in commands:
            commands_file.write(f'{command.file_name()}: {command.build()}\n')

    # By default skip configs whose firmware already exists (resume after an abort);
    # --rebuild forces a full rebuild, archiving the existing outputs to previous/.
    # In skip mode nothing existing is touched, so previous/ is only populated by a
    # full rebuild -- keeping the build dir stable unless you ask for a fresh one.
    if args.rebuild:
        to_build, skipped = commands, []
    else:
        to_build, skipped = [], []
        for command in commands:
            if os.path.exists(destination_for(base_dir, command.file_name())):
                skipped.append(command)
            else:
                to_build.append(command)

    if skipped:
        print(f'Skipping {len(skipped)} firmware(s) already in {base_dir}/ '
              f'(use --rebuild to force a full rebuild)')
    if not to_build:
        print(f'Nothing to build; all {len(commands)} firmwares present.')
        return 0

    total = len(to_build)
    print(f'Building {total} firmwares with {parallel} concurrent build(s) x '
          f'make -j{make_jobs} (detected {cpu} CPUs)')

    start = time.time()
    done = 0
    with ThreadPoolExecutor(max_workers=parallel) as executor:
        futures = [executor.submit(run_build, c, base_dir) for c in to_build]
        try:
            for future in as_completed(futures):
                file_name = future.result()  # re-raises BuildError on failure
                done += 1
                print(f'[{done}/{total}] OK {file_name}  ({time.time() - start:.0f}s)')
        except BuildError as err:
            for future in futures:
                future.cancel()
            print(f'\nBuild failed after {time.time() - start:.0f}s: '
                  f'{err.file_name}: {err} (see {err.log_path})')
            return 1

    elapsed = time.time() - start
    print(f'\nBuilt {total} firmwares in {elapsed:.0f}s ({elapsed / 60:.1f} min); '
          f'{len(commands)} total present.')

    if args.publish:
        return publish(base_dir, commands, args.dry_run)
    return 0

if __name__ == '__main__':
    sys.exit(main())
