// Copyright 2021 @Yowkees
// Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)
// Copyright 2026 Idan Kamara (@idank)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "users/holykeebs/holykeebs.h"

#ifdef VIA_ENABLE
#    include "via.h"
#endif

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
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_MINS  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    KC_LCTL  , KC_LGUI  , KC_LALT  ,LSFT_T(KC_LNG2),LT(1,KC_SPC),LT(3,KC_LNG1),KC_BSPC,LT(2,KC_ENT),LSFT_T(KC_LNG2),KC_RALT,KC_RGUI, KC_RSFT
  ),

  [1] = LAYOUT_universal(
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_RBRC  ,                            KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_F5    , KC_EXLM  , S(KC_6)  ,S(KC_INT3), S(KC_8)  ,                           S(KC_INT1), MS_BTN1  , KC_PGUP  , MS_BTN2  , KC_SCLN  ,
    S(KC_EQL),S(KC_LBRC),S(KC_7)   , S(KC_2)  ,S(KC_RBRC),                            KC_LBRC  , KC_DLR   , KC_PGDN  , MS_BTN3  , KC_F11   ,
    KC_INT1  , KC_EQL   , S(KC_3)  , _______  , _______  , _______  ,      TO(2)    , TO(0)    , _______  , KC_RALT  , KC_RGUI  , KC_F12
  ),

  [2] = LAYOUT_universal(
    KC_TAB   , KC_7     , KC_8     , KC_9     , KC_MINS  ,                            KC_NUHS  , _______  , MS_BTN3  , _______  , KC_BSPC  ,
   S(KC_QUOT), KC_4     , KC_5     , KC_6     ,S(KC_SCLN),                            S(KC_9)  , MS_BTN1  , KC_UP    , MS_BTN2  , KC_QUOT  ,
    KC_SLSH  , KC_1     , KC_2     , KC_3     ,S(KC_MINS),                           S(KC_NUHS), KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  ,
    KC_ESC   , KC_0     , KC_DOT   , KC_DEL   , KC_ENT   , KC_BSPC  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  [3] = LAYOUT_universal(
    RM_TOGG  , HK_AML_T , HK_AML_UP, HK_AML_DN, HK_BONGO_T,                           HK_DUMP  , HK_P_SET_D, HK_P_SET_S, HK_P_SET_THR, HK_SAVE ,
    RM_NEXT  , RM_HUEU  , RM_SATU  , RM_VALU  , KC_UP    ,                            HK_C_SCROLL, HK_I_SCROLL, HK_S_MODE, HK_S_MODE_T, RM_SPDU,
    RM_PREV  , RM_HUED  , RM_SATD  , RM_VALD  , KC_DOWN  ,                            _______  , _______  , HK_D_MODE, HK_D_MODE_T, RM_SPDD ,
    QK_BOOT  , HK_RESET , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , HK_RESET , QK_BOOT
  ),

  [4] = LAYOUT_universal(
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ , _______ ,      _______ ,   _______ , _______ , _______ , _______ , _______
  ),

  [5] = LAYOUT_universal(
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ , _______ ,      _______ ,   _______ , _______ , _______ , _______ , _______
  ),

  [6] = LAYOUT_universal(
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ , _______ ,      _______ ,   _______ , _______ , _______ , _______ , _______
  ),

  [7] = LAYOUT_universal(
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , _______ , _______ , _______ ,      _______ ,   _______ , _______ , _______ , _______ , _______
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Keyball convention: the ball scrolls while the settings layer is held.
    hk_set_dragscroll_both(get_highest_layer(state) == 3);
    return state;
}

#ifdef VIA_ENABLE
// Reflect the runtime-detected ball combination into the VIA "Ball availability"
// layout option (None=0, Right=1, Left=2, Dual=3 -> bits 0-1), so VIA/Remap shows
// the thumb keys for the installed configuration. Called by the userspace once
// detection completes (see hk_pointing_devices_detected_keymap).
void hk_pointing_devices_detected_keymap(bool left_has_pointing, bool right_has_pointing) {
    uint8_t  layouts = (right_has_pointing ? 0x01 : 0x00) | (left_has_pointing ? 0x02 : 0x00);
    uint32_t curr    = via_get_layout_options();
    uint32_t next    = (curr & ~0x3) | layouts;
    if (next != curr) {
        via_set_layout_options(next);
    }
}
#endif
