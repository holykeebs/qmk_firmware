# Repro harness for the keyball61plus "hold-to-snipe sticks on the mouse layer"
# report: a plain custom keycode on the auto-mouse layer, held while the layer
# deactivates, must still deliver its release as the same keycode.
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = custom
MOUSEKEY_ENABLE = no
INTROSPECTION_KEYMAP_C = snipe_hooks.c
