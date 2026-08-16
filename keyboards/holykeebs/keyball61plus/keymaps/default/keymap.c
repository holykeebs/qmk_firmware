// Copyright 2022 @Yowkees
// Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)
// Copyright 2023 Idan Kamara (@idank)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "users/holykeebs/holykeebs.h"

// Mirrors the lily58 hk keymap: QWERTY base, LOWER (symbols + mouse buttons,
// also the auto-mouse layer), RAISE (F-keys + arrows), ADJUST (config) via
// tri-layer. Board-specific deltas:
// - The inner row-3 keys are the scroll wheel presses (left/right click on
//   every layer); lily58's [ ] there move to the bottom corners.
// - LAYOUT_right_ball: the board always ships with a right-side ball, so the
//   three key positions under it aren't part of the keymap. That leaves two
//   right thumb keys (Enter, Upper) and puts Bspace next to the bottom-right
//   bracket.
enum layer_number {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST,
};

// Layer-taps: tapping the layer thumbs gives Esc / Bspace.
#define LOWER LT(_LOWER, KC_ESC)
#define RAISE LT(_RAISE, KC_BSPC)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_right_ball(
    KC_ESC   , KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                                  KC_6     , KC_7     , KC_8     , KC_9     , KC_0     , KC_GRV   ,
    KC_TAB   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                  KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_MINS  ,
    KC_LCTL  , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                                  KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  , KC_QUOT  ,
    // The two inner keys are the encoder (scroll wheel) presses:
    // left wheel = left click, right wheel = right click.
    KC_LSFT  , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     , MS_BTN1  ,              MS_BTN2, KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_RSFT  ,
    KC_LBRC  , KC_LCTL  , KC_LALT  , KC_LGUI  , LOWER    , KC_SPC   , KC_DEL   ,             KC_ENT , RAISE    , KC_BSPC  , KC_RBRC
  ),

  [_LOWER] = LAYOUT_right_ball(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                  _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , KC_EXLM  , KC_AT    , KC_HASH  , KC_DLR   , KC_PERC  ,                                  KC_CIRC  , KC_AMPR  , KC_ASTR  , KC_LPRN  , KC_RPRN  , _______  ,
    _______  , KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                                  KC_6     , KC_7     , KC_8     , KC_9     , KC_0     , _______  ,
    HK_D_MODE, MS_BTN4  , MS_BTN5  , MS_BTN1  , MS_BTN2  , HK_S_MODE, _______  ,            _______  , KC_PIPE  , KC_GRV   , KC_PLUS  , KC_LCBR  , KC_RCBR  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  , _______  ,            _______  , _______  , _______  , _______
  ),

  [_RAISE] = LAYOUT_right_ball(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                  _______  , _______  , _______  , _______  , _______  , _______  ,
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    , KC_F6    ,                                  KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   , KC_F12   ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                  XXXXXXX  , KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , XXXXXXX  ,
    _______  , _______  , _______  , _______  , _______  , _______  , _______  ,            _______  , KC_PLUS  , KC_EQL   , KC_LBRC  , KC_RBRC  , KC_BSLS  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  , _______  ,            _______  , _______  , _______  , _______
  ),

  [_ADJUST] = LAYOUT_right_ball(
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                                  XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,
    QK_BOOT  , HK_DUMP  , HK_SAVE  , HK_RESET , HK_BONGO_T, HK_C_SCROLL,                               UG_TOGG  , UG_NEXT  , UG_HUEU  , UG_SATU  , UG_VALU  , QK_BOOT  ,
    EE_CLR   , HK_P_SET_D, HK_P_SET_S, HK_P_SET_THR, XXXXXXX, HK_S_MODE_T,                             KC_UP    , KC_DOWN  , XXXXXXX  , XXXXXXX  , XXXXXXX  , EE_CLR   ,
    KC_LSFT  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , HK_D_MODE_T, _______ ,            _______  , HK_AML_T , HK_AML_UP, HK_AML_DN, HK_I_SCROLL, XXXXXXX, XXXXXXX  ,
    _______  , _______  , _______  , _______  , _______  , _______  , _______  ,            _______  , _______  , _______  , _______
  ),
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(HK_ENC_SCR_D, HK_ENC_SCR_U), ENCODER_CCW_CW(HK_ENC_SCR_D, HK_ENC_SCR_U) },
    [1] = { ENCODER_CCW_CW(HK_ENC_SCR_D, HK_ENC_SCR_U), ENCODER_CCW_CW(HK_ENC_SCR_D, HK_ENC_SCR_U) },
    [2] = { ENCODER_CCW_CW(HK_ENC_SCR_D, HK_ENC_SCR_U), ENCODER_CCW_CW(HK_ENC_SCR_D, HK_ENC_SCR_U) },
    [3] = { ENCODER_CCW_CW(HK_ENC_SCR_D, HK_ENC_SCR_U), ENCODER_CCW_CW(HK_ENC_SCR_D, HK_ENC_SCR_U) },
};
#endif
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

