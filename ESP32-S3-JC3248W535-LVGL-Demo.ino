#include <Arduino.h>
#include <lvgl.h>
#include "display.h"
#include "esp_bsp.h"

/* Configuración de rotación (90 grados es común para apaisado) */
#define LVGL_PORT_ROTATION_DEGREE (90)

void setup()
{
    Serial.begin(115200);
    Serial.println("Iniciando ESP32-S3 Display...");

    /* 1. Configuración del BSP (Board Support Package) */
    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
        .rotate = LV_DISP_ROT_90, // Ajusta esto si tu pantalla está al revés
    };

    /* 2. Iniciar el hardware y LVGL */
    // Esta función interna ya maneja la memoria y los drivers de pantalla/touch
    lv_disp_t * disp = bsp_display_start_with_config(&cfg);
    
    /* 3. Encender la retroiluminación (Backlight) */
    bsp_display_backlight_on();

    /* 4. Crear tu Interfaz (UI) */
    // IMPORTANTE: Como el BSP usa FreeRTOS, debemos "bloquear" el acceso
    // a LVGL mientras creamos objetos para evitar conflictos.
    bsp_display_lock(0);
    
    setup_my_ui(); // Llamamos a nuestra función de diseño personalizada
    
    bsp_display_unlock();
    
    Serial.println("Interfaz cargada correctamente.");
}

void loop()
{
    // Tu BSP (esp_bsp.c) probablemente ya gestiona el "lv_timer_handler" 
    // en una tarea de FreeRTOS en segundo plano. 
    // Por lo tanto, el loop puede quedar vacío o usarse para tu lógica de sensores/wifi.
    delay(100); 
}

/* ---------------------------------------------------------
   Tu zona de diseño: Aquí es donde empiezas a crear tu UI
   --------------------------------------------------------- */
void setup_my_ui() {
    // Ejemplo: Obtener la pantalla actual
    lv_obj_t *scr = lv_scr_act();

    // 1. Crear una etiqueta (Label) simple
    lv_obj_t *label2 = lv_label_create(scr);
    lv_label_set_text(label2, "JORGE ALBERTO MARTINEZ SUAREZ");
    lv_obj_set_style_text_align(label2, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, -100);
    
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "Hola Mundo!\nESP32-S3");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -20);

    // 2. Crear un botón simple
    lv_obj_t *btn = lv_btn_create(scr);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 40);
    
    // Texto dentro del botón
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Pulsame");
    
    // Añadir un evento (callback) simple
    lv_obj_add_event_cb(btn, [](lv_event_t * e) {
        printf("Boton presionado\n");
    }, LV_EVENT_CLICKED, NULL);
}