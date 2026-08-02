// Copyright 2026
// SPDX-License-Identifier: GPL-2.0-or-later

#include "gtest/gtest.h"
#include "keyboard_report_util.hpp"
#include "mouse_report_util.hpp"
#include "test_common.hpp"
#include "test_pointing_device_driver.h"

extern "C" {
#include "pointing_device_auto_mouse.h"
extern bool     test_sniping_main;
extern bool     test_sniping_peripheral;
extern uint16_t test_last_release_kc;
}

using testing::_;
using testing::AnyNumber;

class AutoMouseSnipe : public TestFixture {
   public:
    void SetUp() override {
        test_sniping_main       = false;
        test_sniping_peripheral = false;
        test_last_release_kc    = 0;
        set_auto_mouse_enable(true);
    }

    // Trip the auto mouse layer with pointer motion.
    void activate_auto_mouse_layer() {
        // Get past the post-keypress/startup AUTO_MOUSE_DELAY window.
        idle_for(1000);
        pd_set_x(50);
        run_one_scan_loop();
        pd_clear_movement();
        run_one_scan_loop();
        ASSERT_TRUE(layer_state_is(1)) << "auto mouse layer did not activate";
    }
};

// The reported repro: snipe key lives on the auto-mouse layer. Ball motion
// raises the layer, the key is pressed and held, the layer times out and
// drops while the key is still down, then the key is released.
TEST_F(AutoMouseSnipe, ReleaseAfterAutoMouseLayerTimesOut) {
    TestDriver driver;
    EXPECT_CALL(driver, send_mouse_mock(_)).Times(AnyNumber());
    EXPECT_CALL(driver, send_keyboard_mock(_)).Times(AnyNumber());

    KeymapKey base_key(0, 0, 0, KC_A);
    KeymapKey snipe_key(1, 0, 0, QK_KB_0);
    set_keymap({base_key, snipe_key});

    activate_auto_mouse_layer();

    // Press and hold the snipe key on the mouse layer.
    snipe_key.press();
    run_one_scan_loop();
    EXPECT_TRUE(test_sniping_main) << "snipe did not engage on press";

    // Let the auto mouse layer time out while the key is still held.
    idle_for(get_auto_mouse_timeout() + 500);
    EXPECT_FALSE(layer_state_is(1)) << "auto mouse layer did not time out";

    // Release: must be delivered as the snipe keycode and clear the mode.
    snipe_key.release();
    run_one_scan_loop();
    EXPECT_EQ(test_last_release_kc, QK_KB_0);
    EXPECT_FALSE(test_sniping_main) << "sniping stuck on after release";
}

// Same, but the layer is still active at release time.
TEST_F(AutoMouseSnipe, ReleaseWhileAutoMouseLayerActive) {
    TestDriver driver;
    EXPECT_CALL(driver, send_mouse_mock(_)).Times(AnyNumber());
    EXPECT_CALL(driver, send_keyboard_mock(_)).Times(AnyNumber());

    KeymapKey base_key(0, 0, 0, KC_A);
    KeymapKey snipe_key(1, 0, 0, QK_KB_0);
    set_keymap({base_key, snipe_key});

    activate_auto_mouse_layer();

    snipe_key.press();
    run_one_scan_loop();
    EXPECT_TRUE(test_sniping_main);

    snipe_key.release();
    run_one_scan_loop();
    EXPECT_FALSE(test_sniping_main) << "sniping stuck on after release";
}

// The shift asymmetry: press without shift (targets main), release while
// shift is held (targets peripheral). Mirrors holding HK_S_MODE and pressing
// shift before letting go.
TEST_F(AutoMouseSnipe, ReleaseWithShiftHeldTargetsOtherSide) {
    TestDriver driver;
    EXPECT_CALL(driver, send_mouse_mock(_)).Times(AnyNumber());
    EXPECT_CALL(driver, send_keyboard_mock(_)).Times(AnyNumber());

    KeymapKey snipe_key(0, 0, 0, QK_KB_0);
    KeymapKey shift_key(0, 1, 0, KC_LSFT);
    set_keymap({snipe_key, shift_key});

    snipe_key.press();
    run_one_scan_loop();
    EXPECT_TRUE(test_sniping_main);

    shift_key.press();
    run_one_scan_loop();

    snipe_key.release();
    run_one_scan_loop();
    shift_key.release();
    run_one_scan_loop();

    EXPECT_FALSE(test_sniping_main) << "sniping stuck on main: release toggled the peripheral instead";
}

// Reverse direction: press with shift (targets peripheral), release after
// letting go of shift first. The natural release order — snipe key last.
TEST_F(AutoMouseSnipe, ShiftedPressReleasedAfterShift) {
    TestDriver driver;
    EXPECT_CALL(driver, send_mouse_mock(_)).Times(AnyNumber());
    EXPECT_CALL(driver, send_keyboard_mock(_)).Times(AnyNumber());

    KeymapKey snipe_key(0, 0, 0, QK_KB_0);
    KeymapKey shift_key(0, 1, 0, KC_LSFT);
    set_keymap({snipe_key, shift_key});

    shift_key.press();
    run_one_scan_loop();
    snipe_key.press();
    run_one_scan_loop();
    EXPECT_TRUE(test_sniping_peripheral);

    shift_key.release();
    run_one_scan_loop();
    snipe_key.release();
    run_one_scan_loop();

    EXPECT_FALSE(test_sniping_peripheral) << "peripheral sniping stuck: release toggled main instead";
}

// Snipe key on a layer held via LT; the LT is released before the snipe key.
TEST_F(AutoMouseSnipe, LayerTapReleasedBeforeSnipeKey) {
    TestDriver driver;
    EXPECT_CALL(driver, send_mouse_mock(_)).Times(AnyNumber());
    EXPECT_CALL(driver, send_keyboard_mock(_)).Times(AnyNumber());

    KeymapKey lt_key(0, 1, 0, LT(1, KC_SPC));
    KeymapKey base_key(0, 0, 0, KC_A);
    KeymapKey snipe_key(1, 0, 0, QK_KB_0);
    set_keymap({lt_key, base_key, snipe_key});

    lt_key.press();
    idle_for(300); // past tapping term, LT resolves as hold
    ASSERT_TRUE(layer_state_is(1));

    snipe_key.press();
    run_one_scan_loop();
    EXPECT_TRUE(test_sniping_main);

    lt_key.release();
    run_one_scan_loop();
    ASSERT_FALSE(layer_state_is(1));

    snipe_key.release();
    run_one_scan_loop();
    EXPECT_EQ(test_last_release_kc, QK_KB_0);
    EXPECT_FALSE(test_sniping_main) << "sniping stuck after LT layer dropped first";
}
