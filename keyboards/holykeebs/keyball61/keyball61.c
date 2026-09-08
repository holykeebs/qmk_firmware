// Copyright 2021 @Yowkees
// Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)
// Copyright 2026 Idan Kamara (@idank)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Duplex matrix mask: marks the unused intersections so QMK ignores them.
// Consumed by the matrix scan because MATRIX_MASKED is defined in config.h.
// clang-format off
matrix_row_t matrix_mask[MATRIX_ROWS] = {
    0b01110111,
    0b01110111,
    0b01110111,
    0b11110111,
    0b11110111,
    0b01110111,
    0b01110111,
    0b01110111,
    0b11110111,
    0b11110111,
};
// clang-format on

#ifdef OLED_ENABLE
#    include "users/holykeebs/holykeebs.h"

// Match the original keyball oledkit rotation: the master renders the info
// panels at the driver's base rotation, the peripheral renders the logo
// rotated 180 (the keyball logo art is drawn for that orientation).
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return is_keyboard_master() ? rotation : OLED_ROTATION_180;
}

// The peripheral half shows the Keyball logo (this board's heritage), matching
// the original keyball lib's oledkit. The master half shows the live info panels
// (handled by the holykeebs userspace). Glyphs 0x80..0xAF in the holykeebs
// logofont are the logo, laid out 3 rows x 16 columns.
void hk_oled_render_secondary(void) {
    char ch = 0x80;
    for (int y = 0; y < 3; y++) {
        oled_write_P(PSTR("  "), false);
        for (int x = 0; x < 16; x++) {
            oled_write_char(ch++, false);
        }
        oled_advance_page(false);
    }
}
#endif
