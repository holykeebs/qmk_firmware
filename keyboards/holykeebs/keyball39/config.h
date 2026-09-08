// Copyright 2021 @Yowkees
// Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)
// Copyright 2026 Idan Kamara (@idank)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define MATRIX_ROWS         (4 * 2)  // split keyboard
#define MATRIX_COLS         6
#define MATRIX_ROW_PINS     { GP29, GP28, GP27, GP26 }
#define MATRIX_COL_PINS     { GP4, GP5, GP6, GP7, GP8, GP9 }
#define DIODE_DIRECTION     COL2ROW
#define MATRIX_MASKED
#define DEBOUNCE            5

// Hand detection: the matrix-grid circuit works on this PCB (unlike keyball61plus),
// so the hand is resolved at boot and the master follows USB; either half can
// be plugged in.
#define SPLIT_HAND_MATRIX_GRID  GP27, GP9
#define SPLIT_HAND_MATRIX_GRID_LOW_IS_LEFT

// PMW3360 over SPI0. Both halves run the driver; the holykeebs userspace detects
// at runtime which side(s) actually carry a ball.
#define SPI_DRIVER     SPID0
#define SPI_SCK_PIN    GP22
#define SPI_MISO_PIN   GP20
#define SPI_MOSI_PIN   GP23
#define PMW33XX_CS_PIN GP21

// RGB Matrix data line. Each half drives its own LED chain from GP0; the LED
// layout in keyboard.json was traced from the KiCad PCBs and describes the
// standard build: the plain PCB on the left (per-key LEDs first, then
// underglow) and the trackball PCB on the right (underglow first, fewer LEDs:
// the keys under the ball are gone). The PCBs are reversible, so a left-ball
// or dual-ball build puts the shorter chain on the other half; RGB Matrix takes
// the split at compile time, so on those builds the animations run in a
// mirrored order and the last LEDs of the plain half stay dark. Cosmetic only.
#define WS2812_DI_PIN GP0

// Hires scroll is on (rules.mk), so mousekey wheel events are one notch each.
#define MOUSEKEY_WHEEL_DELTA 120

#ifdef RGB_MATRIX_ENABLE
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120  // limit for power consumption
#    define RGB_MATRIX_SLEEP                   // turn off LEDs when host is asleep
#endif
