# Corne

*The holykeebs build of foostan's Corne: an RP2040 controller with a pointing device (trackball, TrackPoint, Cirque or Azoteq touchpad) and an OLED on either half.*

This is a self-contained copy of `crkbd/rev1` so it can be carried unchanged in the holykeebs QMK and Vial forks. It keeps the Corne USB identity and matrix, so the stock VIA definition still matches.

* Keyboard Maintainer: [@idank](https://github.com/idank)
* Hardware Supported: [Corne](https://github.com/foostan/crkbd) with an RP2040 controller
* Hardware Availability: *[holykeebs](https://holykeebs.com)*

Make example for this keyboard (after setting up your build environment):

    make holykeebs/corne:default

With a trackball on the right half and the holykeebs OLED:

    make holykeebs/corne:hk -e POINTING_DEVICE=trackball -e POINTING_DEVICE_POSITION=right -e OLED=yes

Flashing example for this keyboard:

    make holykeebs/corne:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,1) in the matrix (the top left key on either half) and plug in the keyboard
* **Physical reset button**: Briefly press the button on the back of the PCB - some may have pads you must short instead
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
