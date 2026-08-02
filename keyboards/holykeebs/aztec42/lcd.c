#include "lcd.h"
#include "qp.h"
#include "qp_comms.h"
#include "qp_st7735.h"
#include "qp_st77xx_opcodes.h"
#include "color.h"
#include "qp_lvgl.h"

painter_device_t display;

bool lcd_init(void) {
    wait_ms(LCD_WAIT_TIME);

    display = qp_st7735_make_spi_device(LCD_HEIGHT, LCD_WIDTH, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, LCD_SPI_DIVISOR, 0);
    if (!qp_init(display, QP_ROTATION_270) || !qp_power(display, true) || !qp_lvgl_attach(display)) return false;

    #ifdef LCD_INVERT_COLOUR
    qp_comms_start(display);
    qp_comms_command(display, ST77XX_CMD_INVERT_ON);
    qp_comms_stop(display);
    #endif

    // Fixes noise around the edges of the display.
    qp_set_viewport_offsets(display, LCD_OFFSET_X, LCD_OFFSET_Y);

    return true;
}

void lcd_draw(void) {
    lv_obj_t * screen = lv_scr_act();

    // Set black background
    lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);

    // Create a label to display "AZTEC42"
    lv_obj_t * aztec_label = lv_label_create(screen);
    lv_label_set_text(aztec_label, "AZTEC42");
    lv_obj_set_style_text_color(aztec_label, lv_color_white(), LV_PART_MAIN);

    // Center the label on the screen
    lv_obj_center(aztec_label);
}

void housekeeping_task_kb(void) {
    if (last_input_activity_elapsed() <= QUANTUM_PAINTER_DISPLAY_TIMEOUT)
    {
        backlight_enable();
    }
    if (last_input_activity_elapsed() > QUANTUM_PAINTER_DISPLAY_TIMEOUT) {
        backlight_disable();
    }
}

void suspend_power_down_kb(void) {
    qp_power(display, false);
    suspend_power_down_user();
}

void suspend_wakeup_init_kb(void) {
    qp_power(display, true);
    suspend_wakeup_init_user();
}
