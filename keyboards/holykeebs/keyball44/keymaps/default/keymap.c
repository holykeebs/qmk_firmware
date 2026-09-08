// Copyright 2021 @Yowkees
// Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)
// Copyright 2026 Idan Kamara (@idank)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "users/holykeebs/holykeebs.h"

// The stock Keyball keymap carried over to the holykeebs userspace. Layers 0-2
// are unchanged. Layer 3 (hold the left inner thumb) keeps its role as the
// settings layer with the Keyball keycodes replaced by their HK_* equivalents:
// the CPI and scroll-divider key pairs become one hold-and-tap key each
// (hold HK_P_SET_D / HK_P_SET_S / HK_P_SET_THR, tap up/down), the scroll-snap
// keys become HK_C_SCROLL (cycles the axis lock), and RGBLIGHT keys become their
// RGB Matrix counterparts. Holding layer 3 also puts the ball into scroll mode,
// as the stock firmware did.

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_ESC   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                        KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_DEL   ,
    KC_TAB   , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                                        KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  , S(KC_7)  ,
    KC_LSFT  , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                                        KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_INT1  ,
              KC_LALT,KC_LGUI,LCTL_T(KC_LNG2)     ,LT(1,KC_SPC),LT(3,KC_LNG1),                  KC_BSPC,LT(2,KC_ENT), RCTL_T(KC_LNG2),     KC_RALT  , KC_PSCR
  ),

  [1] = LAYOUT_universal(
    HK_C_SCROLL ,  KC_F1   , KC_F2    , KC_F3   , KC_F4    , KC_F5    ,                                         KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    HK_I_SCROLL ,  _______ , _______  , KC_UP   , KC_ENT   , KC_DEL   ,                                         KC_PGUP  , MS_BTN1  , KC_UP    , MS_BTN2  , MS_BTN3  , KC_F12   ,
    _______   ,  _______ , KC_LEFT  , KC_DOWN , KC_RGHT  , KC_BSPC  ,                                         KC_PGDN  , KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  , _______  ,
                  _______  , _______ , _______  ,         _______  , _______  ,                   _______  , _______  , _______       , _______  , _______
  ),

  [2] = LAYOUT_universal(
    _______  ,S(KC_QUOT), KC_7     , KC_8    , KC_9     , S(KC_8)  ,                                         S(KC_9)  , S(KC_1)  , S(KC_6)  , KC_LBRC  , S(KC_4)  , _______  ,
    _______  ,S(KC_SCLN), KC_4     , KC_5    , KC_6     , KC_RBRC  ,                                         KC_NUHS  , KC_MINS  , S(KC_EQL), S(KC_3)  , KC_QUOT  , S(KC_2)  ,
    _______  ,S(KC_MINS), KC_1     , KC_2    , KC_3     ,S(KC_RBRC),                                        S(KC_NUHS),S(KC_INT1), KC_EQL   ,S(KC_LBRC),S(KC_SLSH),S(KC_INT3),
                  KC_0     , KC_DOT  , _______  ,         _______  , _______  ,                   KC_DEL   , _______  , _______       , _______  , _______
  ),

  [3] = LAYOUT_universal(
    RM_TOGG  , HK_AML_T , HK_AML_UP, HK_AML_DN, _______  , HK_BONGO_T,                                       HK_DUMP  , RM_SPDU  , RM_SPDD  , _______  , _______  , _______  ,
    RM_NEXT  , RM_HUEU  , RM_SATU  , RM_VALU  , KC_UP    , HK_S_MODE ,                                       HK_P_SET_D, HK_P_SET_S, HK_P_SET_THR, _______, _______  , _______  ,
    RM_PREV  , RM_HUED  , RM_SATD  , RM_VALD  , KC_DOWN  , HK_S_MODE_T,                                      HK_D_MODE, HK_D_MODE_T, _______ , _______  , _______  , HK_SAVE  ,
                  QK_BOOT  , HK_RESET , _______  ,        _______  , _______  ,                   _______  , _______  , _______       , HK_RESET , QK_BOOT
  ),

};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Keyball convention: the ball scrolls while the settings layer is held.
    hk_set_dragscroll_both(get_highest_layer(state) == 3);
    return state;
}
