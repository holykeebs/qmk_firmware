// Copyright 2022 @Yowkees
// Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)
// Copyright 2023 Idan Kamara (@idank)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "users/holykeebs/holykeebs.h"

#ifdef VIA_ENABLE
#    include "via.h"
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_universal(
    KC_ESC   , KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                                  KC_6     , KC_7     , KC_8     , KC_9     , KC_0     , KC_MINS  ,
    KC_DEL   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                  KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_INT3  ,
    KC_TAB   , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                                  KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  , S(KC_7)  ,
    MO(1)    , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     , KC_RBRC  ,              KC_NUHS, KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_RSFT  ,
    _______  , KC_LCTL  , KC_LALT  , KC_LGUI,LT(1,KC_LNG2),LT(2,KC_SPC),LT(3,KC_LNG1),    KC_BSPC,LT(2,KC_ENT),LT(1,KC_LNG2),KC_RGUI, _______ , KC_RALT  , KC_PSCR
  ),

  [1] = LAYOUT_universal(
    S(KC_ESC), S(KC_1)  , KC_LBRC  , S(KC_3)  , S(KC_4)  , S(KC_5)  ,                                  KC_EQL   , S(KC_6)  ,S(KC_QUOT), S(KC_8)  , S(KC_9)  ,S(KC_INT1),
    S(KC_DEL), S(KC_Q)  , S(KC_W)  , S(KC_E)  , S(KC_R)  , S(KC_T)  ,                                  S(KC_Y)  , S(KC_U)  , S(KC_I)  , S(KC_O)  , S(KC_P)  ,S(KC_INT3),
    S(KC_TAB), S(KC_A)  , S(KC_S)  , S(KC_D)  , S(KC_F)  , S(KC_G)  ,                                  S(KC_H)  , S(KC_J)  , S(KC_K)  , S(KC_L)  , KC_QUOT  , S(KC_2)  ,
    _______  , S(KC_Z)  , S(KC_X)  , S(KC_C)  , S(KC_V)  , S(KC_B)  ,S(KC_RBRC),           S(KC_NUHS), S(KC_N)  , S(KC_M)  ,S(KC_COMM), S(KC_DOT),S(KC_SLSH),S(KC_RSFT),
    _______  ,S(KC_LCTL),S(KC_LALT),S(KC_LGUI), _______  , _______  , _______  ,            _______  , _______  , _______  ,S(KC_RGUI), _______  , S(KC_RALT), _______
  ),

  [2] = LAYOUT_universal(
    HK_C_SCROLL, KC_F1  , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                                  KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    HK_D_MODE_T, _______, KC_7     , KC_8     , KC_9     , _______  ,                                  _______  , KC_LEFT  , KC_UP    , KC_RGHT  , _______  , KC_F12   ,
    HK_S_MODE_T, _______, KC_4     , KC_5     , KC_6     ,S(KC_SCLN),                                  KC_PGUP  , MS_BTN1  , KC_DOWN  , MS_BTN2  , MS_BTN3  , _______  ,
    _______  , _______  , KC_1     , KC_2     , KC_3     ,S(KC_MINS), S(KC_8)  ,            S(KC_9)  , KC_PGDN  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , KC_0     , KC_DOT   , _______  , _______  , _______  ,             KC_DEL  , _______  , _______  , _______  , _______  , _______  , _______
  ),

  [3] = LAYOUT_universal(
    UG_TOGG  , HK_AML_T , HK_AML_UP, HK_AML_DN, HK_C_SCROLL, HK_I_SCROLL,                              RGB_M_P  , RGB_M_B  , RGB_M_R  , RGB_M_SW , RGB_M_SN , RGB_M_K  ,
    UG_NEXT  , UG_HUEU  , UG_SATU  , UG_VALU  , _______  , _______  ,                                  RGB_M_X  , RGB_M_G  , RGB_M_T  , RGB_M_TW , _______  , _______  ,
    UG_PREV  , UG_HUED  , UG_SATD  , UG_VALD  , HK_DUMP  , _______  ,                              HK_P_SET_D , HK_P_SET_S, HK_P_SET_THR, HK_S_MODE_T, HK_SAVE, HK_RESET ,
    _______  , _______  ,HK_D_MODE_T, HK_D_MODE, KC_LSFT , _______  , EE_CLR   ,            EE_CLR   , KC_HOME  , KC_PGDN  , KC_PGUP  , KC_END   , _______  , _______  ,
    QK_BOOT  , _______  , KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , _______  ,            _______  , KC_BSPC  , _______  , _______  , _______  , _______  , QK_BOOT
  ),

  [4] = LAYOUT_universal(
    _______  , _______   , _______  , _______  , _______  , _______  ,                                  _______  , _______  , _______  , _______ , _______ , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                  _______  , _______  , _______  , _______ , _______  , _______  ,
    _______ , _______  , _______  , _______  , _______  , _______  ,                                  _______  , _______ , _______ , _______  , _______ , _______  ,
    _______  , _______  , _______ , _______ , _______  , _______  , _______   ,            _______   , _______  , _______  , _______  , _______   , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______    , _______  , _______  ,            _______  , _______  , _______  , _______  , _______  , _______  , _______
  ),

  [5] = LAYOUT_universal(
    _______  , _______   , _______  , _______  , _______  , _______  ,                                  _______  , _______  , _______  , _______ , _______ , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                  _______  , _______  , _______  , _______ , _______  , _______  ,
    _______ , _______  , _______  , _______  , _______  , _______  ,                                  _______  , _______ , _______ , _______  , _______ , _______  ,
    _______  , _______  , _______ , _______ , _______  , _______  , _______   ,            _______   , _______  , _______  , _______  , _______   , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______    , _______  , _______  ,            _______  , _______  , _______  , _______  , _______  , _______  , _______
  ),

  [6] = LAYOUT_universal(
    _______  , _______   , _______  , _______  , _______  , _______  ,                                  _______  , _______  , _______  , _______ , _______ , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                  _______  , _______  , _______  , _______ , _______  , _______  ,
    _______ , _______  , _______  , _______  , _______  , _______  ,                                  _______  , _______ , _______ , _______  , _______ , _______  ,
    _______  , _______  , _______ , _______ , _______  , _______  , _______   ,            _______   , _______  , _______  , _______  , _______   , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______    , _______  , _______  ,            _______  , _______  , _______  , _______  , _______  , _______  , _______
  ),

  [7] = LAYOUT_universal(
    _______  , _______   , _______  , _______  , _______  , _______  ,                                  _______  , _______  , _______  , _______ , _______ , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                  _______  , _______  , _______  , _______ , _______  , _______  ,
    _______ , _______  , _______  , _______  , _______  , _______  ,                                  _______  , _______ , _______ , _______  , _______ , _______  ,
    _______  , _______  , _______ , _______ , _______  , _______  , _______   ,            _______   , _______  , _______  , _______  , _______   , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______    , _______  , _______  ,            _______  , _______  , _______  , _______  , _______  , _______  , _______
  ),
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(MS_WHLD, MS_WHLU), ENCODER_CCW_CW(MS_WHLD, MS_WHLU) },
    [1] = { ENCODER_CCW_CW(MS_WHLD, MS_WHLU), ENCODER_CCW_CW(MS_WHLD, MS_WHLU) },
    [2] = { ENCODER_CCW_CW(MS_WHLD, MS_WHLU), ENCODER_CCW_CW(MS_WHLD, MS_WHLU) },
    [3] = { ENCODER_CCW_CW(MS_WHLD, MS_WHLU), ENCODER_CCW_CW(MS_WHLD, MS_WHLU) },
    [4] = { ENCODER_CCW_CW(MS_WHLD, MS_WHLU), ENCODER_CCW_CW(MS_WHLD, MS_WHLU) },
    [5] = { ENCODER_CCW_CW(MS_WHLD, MS_WHLU), ENCODER_CCW_CW(MS_WHLD, MS_WHLU) },
    [6] = { ENCODER_CCW_CW(MS_WHLD, MS_WHLU), ENCODER_CCW_CW(MS_WHLD, MS_WHLU) },
    [7] = { ENCODER_CCW_CW(MS_WHLD, MS_WHLU), ENCODER_CCW_CW(MS_WHLD, MS_WHLU) },
};
#endif
// clang-format on

// keyball parity: holding the layer-3 thumb key (LT(3,...)) switches the ball(s)
// to scroll — the manual-scroll counterpart to the auto-mouse cursor layer (2).
// Mirrors keyball's keyball_set_scroll_mode(get_highest_layer() == 3).
layer_state_t layer_state_set_user(layer_state_t state) {
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
