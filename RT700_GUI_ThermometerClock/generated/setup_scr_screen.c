/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen(lv_ui *ui)
{
    //Write codes screen
    ui->screen = lv_obj_create(NULL);
    /* CO5300 circular panel visible framebuffer: 466 x 466. */
    lv_obj_set_size(ui->screen, 466, 466);
    lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_bg_color(ui->screen, lv_color_hex(0xadd8e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_bg_grad_dir(ui->screen, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_obj_set_style_bg_grad_color(ui->screen, lv_color_hex(0x343434), LV_PART_MAIN|LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui->screen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_main_stop(ui->screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_Clock_Back
    ui->screen_Clock_Back = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_Clock_Back, 92, 138);
    lv_obj_set_size(ui->screen_Clock_Back, 293, 76);
    lv_label_set_text(ui->screen_Clock_Back, "11:25 AM");
    lv_label_set_long_mode(ui->screen_Clock_Back, LV_LABEL_LONG_WRAP);

    //Write style for screen_Clock_Back, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_Clock_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_Clock_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_Clock_Back, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_Clock_Back, &lv_font_Abel_regular_70, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_Clock_Back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_Clock_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_Clock_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_Clock_Back, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_Clock_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_Clock_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_Clock_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_Clock_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_Clock_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_Clock_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_Clock_Front
    ui->screen_Clock_Front = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_Clock_Front, 90, 136);
    lv_obj_set_size(ui->screen_Clock_Front, 293, 76);
    lv_label_set_text(ui->screen_Clock_Front, "11:25 AM");
    lv_label_set_long_mode(ui->screen_Clock_Front, LV_LABEL_LONG_WRAP);

    //Write style for screen_Clock_Front, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_Clock_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_Clock_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_Clock_Front, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_Clock_Front, &lv_font_Abel_regular_70, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_Clock_Front, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_Clock_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_Clock_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_Clock_Front, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_Clock_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_Clock_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_Clock_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_Clock_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_Clock_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_Clock_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_TempUnit_Back
    ui->screen_TempUnit_Back = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_TempUnit_Back, 248, 238);
    lv_obj_set_size(ui->screen_TempUnit_Back, 96, 84);
    lv_label_set_text(ui->screen_TempUnit_Back, "°C");
    lv_label_set_long_mode(ui->screen_TempUnit_Back, LV_LABEL_LONG_WRAP);

    //Write style for screen_TempUnit_Back, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_TempUnit_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_TempUnit_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_TempUnit_Back, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_TempUnit_Back, &lv_font_Abel_regular_80, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_TempUnit_Back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_TempUnit_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_TempUnit_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_TempUnit_Back, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_TempUnit_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_TempUnit_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_TempUnit_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_TempUnit_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_TempUnit_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_TempUnit_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_TempUnit_Front
    ui->screen_TempUnit_Front = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_TempUnit_Front, 246, 236);
    lv_obj_set_size(ui->screen_TempUnit_Front, 96, 84);
    lv_label_set_text(ui->screen_TempUnit_Front, "°C");
    lv_label_set_long_mode(ui->screen_TempUnit_Front, LV_LABEL_LONG_WRAP);

    //Write style for screen_TempUnit_Front, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_TempUnit_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_TempUnit_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_TempUnit_Front, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_TempUnit_Front, &lv_font_Abel_regular_80, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_TempUnit_Front, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_TempUnit_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_TempUnit_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_TempUnit_Front, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_TempUnit_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_TempUnit_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_TempUnit_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_TempUnit_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_TempUnit_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_TempUnit_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_TempDigits_Back
    ui->screen_TempDigits_Back = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_TempDigits_Back, 129, 238);
    lv_obj_set_size(ui->screen_TempDigits_Back, 140, 83);
    lv_label_set_text(ui->screen_TempDigits_Back, "25.0");
    lv_label_set_long_mode(ui->screen_TempDigits_Back, LV_LABEL_LONG_WRAP);

    //Write style for screen_TempDigits_Back, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_TempDigits_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_TempDigits_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_TempDigits_Back, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_TempDigits_Back, &lv_font_Abel_regular_80, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_TempDigits_Back, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_TempDigits_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_TempDigits_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_TempDigits_Back, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_TempDigits_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_TempDigits_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_TempDigits_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_TempDigits_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_TempDigits_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_TempDigits_Back, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_TempDigits_Front
    ui->screen_TempDigits_Front = lv_label_create(ui->screen);
    lv_obj_set_pos(ui->screen_TempDigits_Front, 127, 236);
    lv_obj_set_size(ui->screen_TempDigits_Front, 140, 84);
    lv_label_set_text(ui->screen_TempDigits_Front, "25.0");
    lv_label_set_long_mode(ui->screen_TempDigits_Front, LV_LABEL_LONG_WRAP);

    //Write style for screen_TempDigits_Front, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_TempDigits_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_TempDigits_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_TempDigits_Front, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_TempDigits_Front, &lv_font_Abel_regular_80, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_TempDigits_Front, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_TempDigits_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_TempDigits_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_TempDigits_Front, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_TempDigits_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_TempDigits_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_TempDigits_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_TempDigits_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_TempDigits_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_TempDigits_Front, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen);

}
