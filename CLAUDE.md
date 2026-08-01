# CLAUDE.md

Notes for working in this fork. The upstream QMK docs cover the rest; this file
documents the holykeebs-specific build workflow.

## Compiling a holykeebs keyboard

This fork's keyboards are driven by the `users/holykeebs` userspace, which
implements the pointing device handling (Pimoroni trackball, TrackPoint, Cirque,
Azoteq TPS, PMW3360), sniping, drag-scroll, scroll lock, OLED, EEPROM config, and
split state sync. Boards select behavior at build time via `-e VAR=value` flags.

### The userspace is an external overlay (one-time setup)

`users/holykeebs` is **not committed to this repo**. It's the single source of
truth shared with the vial-qmk fork, kept in a separate [QMK External Userspace](
https://docs.qmk.fm/newbs_external_userspace) overlay repo at
`../holykeebs-userspace` (where `users/holykeebs/` actually lives).

Point the `qmk` CLI at it once — the setting is global, so it applies to every
fork you build:

```
qmk config user.overlay_dir="/home/idank/dev/qmk/holykeebs-userspace"
```

The build prefers `<overlay>/users/holykeebs` over any in-tree copy. If the
overlay isn't configured, builds fail loudly with
`fatal error: users/holykeebs/holykeebs.h: No such file or directory` — set
`user.overlay_dir` (or prefix the build with `QMK_USERSPACE=/path make ...`).
Edit the userspace in the overlay repo, not here.

### Command form

```
make <keyboard>:<keymap> -e USER_NAME=holykeebs -e POINTING_DEVICE=<dev> -e POINTING_DEVICE_POSITION=<pos> [-e MORE=val ...]
```

Verified example — single trackball on the right half:

```
make holykeebs/sweeq:hk -e USER_NAME=holykeebs -e POINTING_DEVICE=trackball -e POINTING_DEVICE_POSITION=right
```

Append `:flash` to the target to flash, or copy the resulting `<target>.uf2`
to the RP2040 bootloader drive.

### `USER_NAME=holykeebs` is mandatory (and the failure is silent)

The keymaps include the userspace header by full path
(`#include "users/holykeebs/holykeebs.h"`). So if you omit `USER_NAME`, the
build still **compiles and links successfully** — the header resolves the
`HK_*` keycode enum, and QMK's weak default hooks satisfy the linker — but the
firmware is **silently missing the entire userspace**: none of
`users/holykeebs/*.c` is compiled, so there's no pointing-device processing, no
`HK_*` keycode handling, and no EEPROM config.

`USER_NAME=holykeebs` is what pulls in `users/holykeebs/rules.mk`, which:
- adds `holykeebs.c` / `oled.c` / etc. to the build, and
- sets `SERIAL_DRIVER = vendor` for splits.

Without it, a split board instead fails later with
`SOFT_SERIAL_PIN undeclared` in `platforms/chibios/drivers/serial.c` (the
serial driver was never switched off the soft-serial default). If you see that
error, you forgot `USER_NAME=holykeebs`.

### Keyboards and keymaps

Boards driven by this userspace: `holykeebs/sweeq`, `holykeebs/spankbd`,
`holykeebs/aztec42`, `holykeebs/keyball61plus`, `crkbd/rev1`, `lily58/rev1`.

Keymap: use `hk` when a pointing device is present (it adds the pointer/scroll
layer); `via` for a build with no pointing device. (Some boards, e.g. aztec42,
use `default`/`vial` instead; keyball61plus uses `via`/`default` — see below.)

### keyball61plus (fixed dual PMW3360, runtime detection)

Unlike the modular boards, `holykeebs/keyball61plus` takes **no `POINTING_DEVICE`
var**. It's always built dual-PMW3360 combined; the board `rules.mk` declares the
fixed pointing config, and the firmware detects at runtime which side(s) actually
have a ball — one image covers left / right / dual — and updates the VIA "Ball
availability" layout to match. Handedness comes from `SPLIT_HAND_MATRIX_GRID` and
the master from USB/VBUS, so no master side is pinned and USB can go to either half.

```
make holykeebs/keyball61plus:via -e USER_NAME=holykeebs -e OLED=yes
```

Keymaps: `via` (drives the dynamic ball layout) or `default`. `OLED=yes` shows the
info panels on the master and the Keyball logo on the peripheral. Hires scroll is
**on by default** for this board (its `rules.mk` forces `HK_HIRES_SCROLL`; the board
is wired for it). Hold the layer-3 thumb key to scroll.

### Build variables

| Variable | Values | Notes |
|---|---|---|
| `USER_NAME` | `holykeebs` | **Required** — see above. |
| `POINTING_DEVICE` | `trackball`, `trackpoint`, `cirque35`, `cirque40`, `tps43`, `tps65`, or `<left>_<right>` (e.g. `trackball_cirque40`) | Omit for no pointing device. Full valid set: `VALID_POINTING_DEVICE_CONFIGURATIONS` in `users/holykeebs/rules.mk`. |
| `POINTING_DEVICE_POSITION` | `right`, `left`, `thumb`, `thumb_inner`, `thumb_outer`, `middle` | Where the (single) device sits. |
| `SIDE` | `left`, `right` | Required for dual (`<left>_<right>`) configs — build once per side. |
| `OLED` | `yes` (holykeebs OLED: status + keylog), `stock` (plain QMK OLED) | Omit for none. |
| `OLED_FLIP` | `yes` | Stock-OLED keymaps only: render the status panel on the off-hand half instead of the master. No effect with `OLED=yes` (the holykeebs OLED renders both halves at the driver's base rotation — all these boards mount both halves' OLEDs the same way, so no flip is needed or applied). |
| `BONGO_ENABLE` | `no` | Bongocat OLED animation. Compiled into `OLED=yes` builds by default and toggled at runtime (`HK_BONGO_TOGGLE`; hold shift to target the peripheral); set `no` to drop it. Defaults to showing the info panels. |
| `TRACKBALL_RGB_RAINBOW` | `yes` | Rainbow-cycle the Pimoroni trackball LED. |
| `HIRES_SCROLL` | `yes` | High-resolution (sub-line) scrolling. |
| `CONSOLE` | `yes` | Enable console for debug (`HK_DUMP`, `printf`). |

The master/peripheral side is derived automatically (`rules.mk` sets
`MASTER_SIDE` → `HK_MASTER_LEFT`/`HK_MASTER_RIGHT`); don't set it by hand.
keyball61plus is the exception: it resolves handedness from its matrix grid and
the master from USB, so no master side is pinned (the userspace skips the forced
`MASTER_*` when the board has its own `SPLIT_HAND_MATRIX_GRID`).

### Bulk builds

`build_all.py` compiles the full board × pointing-device × OLED matrix. It
always passes `USER_NAME=holykeebs` and a unique `TARGET` per build (so parallel
builds don't clobber each other). Run `python3 build_all.py` (`--help` for jobs
/ parallelism).

The matrix also covers the Vial firmware, which builds inside the sibling
`../vial-qmk` checkout (`HK_VIAL_QMK` overrides the path): `keyball61plus:vial`
plus `keyball/keyball{39,44,61}:vial`. Those keyball boards keep the stock
Keyball firmware there and take no build vars.

`python3 build_all.py --publish` is the formal release flow: it requires this
repo **and** the overlay to be on `hk-master`, clean, and in sync with origin,
forces a full rebuild, then syncs the matrix to the
`holykeebs/qmk_compiled` `latest` release (uploads everything, prunes stale
remote assets, preserves externally-built ones like the killerwhale file, and
refreshes the release notes with source provenance). Add `--dry-run` to see the
upload/prune plan without touching the release.
