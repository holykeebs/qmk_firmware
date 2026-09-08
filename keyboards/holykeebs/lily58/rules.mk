# This board is driven by the holykeebs userspace; every keymap needs it. Set it
# here so bare builds (and CI's qmk mass-compile, which passes no -e vars) work.
# Without it USER_NAME defaults to the keymap name and the userspace sources are
# silently left out: the split serial driver stays on the soft-serial default
# and the build fails on SOFT_SERIAL_PIN.
USER_NAME = holykeebs
