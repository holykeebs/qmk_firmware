# This board is driven by the holykeebs userspace; every keymap needs it. Set it
# here so bare builds (and CI's qmk mass-compile, which passes no -e vars) work.
# Without it USER_NAME defaults to the keymap name and the userspace sources are
# silently left out, failing at link on the hk_* symbols the board code uses.
USER_NAME = holykeebs

# Build Options
BOOTMAGIC_ENABLE = no       # Enable Bootmagic Lite
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = no            # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
AUDIO_ENABLE = no           # Audio output

# Duplex matrix. Private copy under lib/ so the released keyball61 (which keeps
# its own copy under keyboards/keyball/lib) is unaffected by changes here.
CUSTOM_MATRIX = lite
SRC += lib/duplexmatrix/duplexmatrix.c

# Split keyboard.
SERIAL_DRIVER = vendor

# PMW3360 optical trackball sensor. Unlike the modular holykeebs boards (which
# pick a sensor per half via the POINTING_DEVICE=<left>_<right> matrix), the
# keyball61plus is always built dual-PMW3360-capable and combined: both halves
# run the PMW3360 driver regardless of how many balls are physically installed.
# The count is detected at runtime (which side has a ball), so the board declares
# its fixed pointing config here rather than via a POINTING_DEVICE build var. The
# userspace only recognizes POINTER_KIND_PMW3360 (kind + CPI + rotation).
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = pmw3360
OPT_DEFS += -DHK_POINTING_DEVICE_LEFT_PMW3360 -DHK_POINTING_DEVICE_RIGHT_PMW3360
OPT_DEFS += -DSPLIT_POINTING_ENABLE -DPOINTING_DEVICE_COMBINED

# High-resolution (sub-line) scrolling on by default. The board is wired for it
# (MOUSEKEY_WHEEL_DELTA 120 in config.h), and it gives the proportional, smooth
# scroll the trackball expects — the holykeebs scroll buffer becomes a divisor
# (resolution/size) instead of the coarser fixed ±1-per-report debounce.
OPT_DEFS += -DHK_HIRES_SCROLL

MOUSEKEY_ENABLE = yes

# Rotary encoder (one per side, declared in keyboard.json).
ENCODER_MAP_ENABLE = yes

# Per-key RGB lighting. Enable per keymap.
RGB_MATRIX_ENABLE = no
RGB_MATRIX_DRIVER = ws2812

# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend

# OLED is provided by the holykeebs userspace (enable with OLED=yes).
OLED_ENABLE = no

# Disable other features to squeeze firmware size
SPACE_CADET_ENABLE = no
MAGIC_ENABLE = no
