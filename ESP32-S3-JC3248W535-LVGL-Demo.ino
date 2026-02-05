#include <Arduino.h>
#include <lvgl.h>
#include "src/esp_bsp.h"
#include "src/display.h"

void setup() {
    Serial.begin(115200);

    // 1. Inicialización de Hardware y LVGL
    // El BSP configura los comandos internos para que no haya RUIDO.
    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
        .rotate = LV_DISP_ROT_NONE, // Sugerencia del compilador aplicada
    };

    // Esto inicia Pantalla, Touch y la tarea de LVGL en segundo plano
    bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();

    // 2. Diseño de la Interfaz
    bsp_display_lock(0); // Bloqueo de seguridad para FreeRTOS
    
        setup_main_screen();
        
    bsp_display_unlock();
}

void loop() {
    // El loop queda libre para tu lógica (sensores, wifi, etc.)
    // LVGL se refresca solo gracias al BSP.
    delay(1000);
}

// --- Tu zona de diseño personalizada ---
void setup_main_screen() {
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1A1A1A), 0); // Fondo oscuro
    lv_obj_set_style_text_font(scr, &lv_font_montserrat_18, 0);

    // Botón interactivo
    lv_obj_t *btn = lv_btn_create(scr);
    lv_obj_set_size(btn, 150, 60);
    lv_obj_center(btn);
    
    // Label dentro del botón
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "¡Púlsame!");
    lv_obj_center(label);

    // Evento del botón (para probar el Touch)
    lv_obj_add_event_cb(btn, [](lv_event_t * e) {
        static uint32_t cnt = 0;
        lv_obj_t * b = lv_event_get_target(e);
        lv_obj_t * l = lv_obj_get_child(b, 0);
        lv_label_set_text_fmt(l, "Toques: %d", ++cnt);
        Serial.println("¡Touch funcionando!");
    }, LV_EVENT_CLICKED, NULL);
}