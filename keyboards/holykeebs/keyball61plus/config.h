/*
Copyright 2021 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// This board lives under keyboards/holykeebs/ on purpose: it inherits no
// shared parent config.h, so all pointing/scroll/OLED/EEPROM behavior comes
// from the holykeebs userspace (built with USER_NAME=holykeebs). This file
// holds only the board's own hardware.

// Key matrix parameters (duplex matrix).
#define MATRIX_ROWS         (5 * 2)  // split keyboard
#define MATRIX_COLS         (4 * 2)  // duplex matrix
#define MATRIX_ROW_PINS     { GP4, GP5, GP6, GP7, GP8 }
#define MATRIX_COL_PINS     { GP29, GP28, GP27, GP26 }
#define MATRIX_MASKED
#define DEBOUNCE            5

// Split hand detection. The PCB has a matrix-grid circuit (lROW2/lCOL3 via
// lD33, left half only), but the sense path crosses two diodes in series
// (lD33 plus the shared duplex column diode lD37), leaving the read pin at
// ~1.0-1.4V — marginal against the RP2040 input threshold — so
// SPLIT_HAND_MATRIX_GRID GP26, GP6 misreads on production boards. Until a PCB
// rev lands the hand diode directly on the column pin, detect the hand from
// the fitted PMW3360 instead (this variant only ships a right-side ball); see
// users/holykeebs/hand.c. Serial driver/pin and split behavior (watchdog) are
// provided by the holykeebs userspace.
#define HK_HAND_FROM_POINTER

// PMW3360 trackball sensor wiring (SPI). The sensor's driver, rotation, CPI and
// scroll behavior are owned by the holykeebs userspace. Physical orientation
// (for the PMW3360 device kind, once added): left ball ROTATION_90 + INVERT_Y,
// right ball ROTATION_270 + INVERT_Y.
#define SPI_DRIVER     SPID0
#define SPI_SCK_PIN    GP22
#define SPI_MISO_PIN   GP20
#define SPI_MOSI_PIN   GP23
#define PMW33XX_CS_PIN GP21

// RGB Matrix data line.
#define WS2812_DI_PIN GP0

// Encoder press/release gap. encoder.c takes this delay twice per detent as a
// blocking wait, stalling the matrix, the pointing device and the OLED with it,
// so the wheels scroll through HK_ENCODER_SCROLL_UP/DOWN instead of mousekey:
// those send nothing at key press time (the scroll rides the pointing-device
// report), which is what the delay was guarding against — Windows drops the
// wheel event when the v=1 and v=0 reports arrive together.
#define ENCODER_MAP_KEY_DELAY 0

// The right half's encoder footprint is mirrored, so the same pin order decodes
// backwards there: keyboard.json swaps pin_a/pin_b for the right rotary only.
// Verified on hardware — with both halves on the same order the wheels scroll
// opposite ways; with the swap, spinning either wheel toward you decodes
// counter-clockwise, which the encoder map turns into scroll down.

// Hires scroll (enabled via the holykeebs userspace) tells the OS to divide
// wheel deltas by 120. mousekey's default wheel_unit() is 1, which becomes
// 1/120 of a scroll line — invisible. Bump the base wheel delta so MS_WHLU/
// MS_WHLD send one full hires notch, for keymaps that bind them directly.
#define MOUSEKEY_WHEEL_DELTA 120

// RGB Matrix settings
// Left:  32 per-key + 6 underglow = 38
// Right: 29 per-key + 6 underglow = 35
#ifdef RGB_MATRIX_ENABLE
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120  // limit for power consumption
#    define RGB_MATRIX_SLEEP                   // turn off LEDs when host is asleep
#endif
