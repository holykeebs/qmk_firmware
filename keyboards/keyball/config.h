#define WS2812_DI_PIN GP0
#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_RAINBOW_SWIRL
#endif

#ifndef OLED_FONT_H
#    define OLED_FONT_H "keyboards/keyball/lib/logofont/logofont.c"
#    define OLED_FONT_START 32
#    define OLED_FONT_END 195
#endif

#define USB_VBUS_PIN GP19

#define SPLIT_WATCHDOG_ENABLE
#define SPLIT_WATCHDOG_TIMEOUT 3000

// Use int16_t for mouse x/y reports so the full PMW3360 sensor range
// (-32768..32767) is preserved. Without this, QMK clips sensor deltas to
// int8_t (-128..127) per cycle, losing motion data during fast movements.
#define MOUSE_EXTENDED_REPORT

// Use int16_t for scroll h/v reports and advertise a 1/120 resolution
// multiplier so the OS applies smooth, sub-line scrolling.
#define WHEEL_EXTENDED_REPORT
#define POINTING_DEVICE_HIRES_SCROLL_ENABLE

#define SPI_DRIVER SPID0
#define SPI_SCK_PIN GP22
#define SPI_MISO_PIN GP20
#define SPI_MOSI_PIN GP23
#define PMW33XX_CS_PIN GP21
#define SPLIT_POINTING_ENABLE
#define POINTING_DEVICE_COMBINED

// Rotation settings for left side trackball.
#define POINTING_DEVICE_ROTATION_90
#define POINTING_DEVICE_INVERT_Y

// Rotation settings for right side trackball.
#define POINTING_DEVICE_ROTATION_270_RIGHT
#define POINTING_DEVICE_INVERT_Y_RIGHT

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

// Restores VIA key tester matrix readout disabled by qmk/qmk_firmware#25414.
// TODO: consider switching to SECURE_ENABLE for a safer unlock-based approach.
#define VIA_INSECURE
