# Keyball39

*The holykeebs build of Yowkees' Keyball39: RP2040, a 34 mm trackball on either or both halves, OLEDs, per-key RGB Matrix.*

This board runs the holykeebs userspace (pointing, scrolling, OLED, settings) and the same `HK_*` keycodes as every other holykeebs keyboard, not the original Keyball firmware. One image covers a ball on the left, the right or both: the firmware detects which halves carry one at boot and updates the VIA "Ball availability" layout option to match. Either half can take the USB cable.

* Keyboard Maintainer: [@idank](https://github.com/idank)
* Hardware Supported: [Keyball39](https://github.com/Yowkees/keyball) PCB with an RP2040 controller
* Hardware Availability: *[holykeebs](https://holykeebs.com)*

The RGB Matrix LED layout was traced from the PCB files and describes the standard build with the trackball PCB on the right. A left-ball or dual-ball build lights up too, but its animations run in a mirrored order.

Make example for this keyboard (after setting up your build environment):

    make holykeebs/keyball39:via -e OLED=yes

Flashing example for this keyboard:

    make holykeebs/keyball39:via:flash -e OLED=yes

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Double tap reset**: Press the reset button on the PCB twice within a second
* **Physical reset button**: Hold the boot button while pressing reset
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
