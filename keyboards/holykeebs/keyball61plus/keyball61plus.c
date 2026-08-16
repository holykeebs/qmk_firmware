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

#if defined(RGB_MATRIX_ENABLE) && !defined(HK_WHEEL_LED)
// Hold the LED under each scroll wheel dark. The wheel sits on the inner row-3
// key of its half ([3,7] left, [8,7] right), and its body scatters that LED
// rather than lighting a legend the way the other per-key LEDs do. Build with
// WHEEL_LED=yes to light it anyway.
//
// Done as an indicator rather than by clearing the LED's flags, because that
// only stops the effects that honor flags: an effect calling
// rgb_matrix_set_color_all would light it anyway. Running last, after both the
// effect and any keymap indicator, is what makes it unconditional.
bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    const bool cont = rgb_matrix_indicators_advanced_user(led_min, led_max);

    const uint8_t wheel_leds[] = {
        g_led_config.matrix_co[3][7],
        g_led_config.matrix_co[8][7],
    };
    for (uint8_t i = 0; i < ARRAY_SIZE(wheel_leds); i++) {
        const uint8_t led = wheel_leds[i];
        if (led != NO_LED && led >= led_min && led < led_max) {
            rgb_matrix_set_color(led, 0, 0, 0);
        }
    }

    return cont;
}
#endif
