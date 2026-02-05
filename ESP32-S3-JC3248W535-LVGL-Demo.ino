#include <Arduino.h>
#include <lvgl.h>
#include "src/esp_bsp.h"
#include "src/display.h"

// 1. Declaramos la función de eventos igual que en la documentación de LVGL
static void btn_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e); // Obtenemos el objeto que disparó el evento

    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        // Obtenemos el primer hijo del botón (que es el label) para cambiar su texto
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Clics: %d", cnt);
        
        Serial.println("Evento: Click detectado");
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        Serial.println("Evento: Valor cambiado (Toggle)");
    }
}

void setup() {
    Serial.begin(115200);

    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
        .rotate = LV_DISP_ROT_NONE,
    };

    bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();

    bsp_display_lock(0);
        setup_main_screen();
    bsp_display_unlock();
}

void setup_main_screen() {
    lv_obj_t * scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1A1A1A), 0);
    lv_obj_set_style_text_font(scr, &lv_font_montserrat_18, 0);

    /* CREACIÓN DEL BOTÓN */
    lv_obj_t * btn = lv_btn_create(scr);
    lv_obj_set_size(btn, 150, 60);
    lv_obj_center(btn);
    
    // Si quieres que el botón sea tipo "Toggle" (se quede hundido), descomenta esta línea:
    // lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);

    /* ASIGNACIÓN DEL EVENTO (Igual que en la documentación) */
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_ALL, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Púlsame");
    lv_obj_center(label);
}

void loop() {
    delay(1000);
}