// Copyright 2026
// SPDX-License-Identifier: GPL-2.0-or-later

// Mimics the holykeebs userspace hold-to-snipe handler (HK_SNIPING_MODE):
// pressed -> mode on, released -> mode off, with the shift-targets-peripheral
// side latched at press, exactly like process_record_user does in
// users/holykeebs/holykeebs.c. Keep the two in sync.

#include "quantum.h"

bool     test_sniping_main       = false;
bool     test_sniping_peripheral = false;
uint16_t test_last_release_kc    = 0;

static bool has_shift_mod(void) {
    return mod_config(get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        test_last_release_kc = keycode;
    }
    if (keycode == QK_KB_0) { // stand-in for HK_SNIPING_MODE
        static bool snipe_side_peripheral = false;
        if (record->event.pressed) {
            snipe_side_peripheral = has_shift_mod();
        }
        if (snipe_side_peripheral) {
            test_sniping_peripheral = record->event.pressed;
        } else {
            test_sniping_main = record->event.pressed;
        }
        return false;
    }
    return true;
}
