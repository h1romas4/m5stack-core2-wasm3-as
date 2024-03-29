#include "M5Core2.h"

#include "test_freetype.h"

#ifdef CONFIG_WASM_CLOCK
#include "test_nvs_wifi.h"
#include "test_wasm3_clockenv.h"
#endif
#ifdef CONFIG_WASM_3DCUBE
#include "test_wasm3_imu6886.h"
#endif
#ifdef CONFIG_WASM_3DCUBE_IMU6886
#include "test_bluetooth_serial.h"
#include "test_i2c_imu6866.h"
#include "test_wasm3_imu6886.h"
#endif
#ifdef CONFIG_WASM_UNITGPS
#include "test_uart_unitgps.h"
#include "test_wasm3_gpsgsv.h"
#endif

static const char *TAG = "main.cpp";

/**
 * FreeType member
 */
font_render_t font_render;

/**
 * Wasm3 member
 */
boolean enable_wasm = false;

void setup(void)
{
    M5.begin();

    // Test - Load FreeType font to PSRAM and init
    load_ttf_spiffs_psram();
    // load_ttf_spiffs();

    // Test FreeType
    font_render = create_freetype_render(/* font size */ 40, /* font cache */ 24);

    draw_freetype_string("M5Stack Core2", 10, 10 + 50, M5.Lcd.color565(0, 0, 255), &font_render);
    draw_freetype_string("Development", 10, 10 + 50 * 2, M5.Lcd.color565(255, 255, 255), &font_render);
    draw_freetype_string("Board", 10, 10 + 50 * 3, M5.Lcd.color565(255, 255, 255), &font_render);
    draw_freetype_string("Xtensa", 10, 10 + 50 * 4, M5.Lcd.color565(255, 0, 0), &font_render);

    // Test NVS and Wifi
    // BT and Wifi will run out of IRAM if used at the same time.
    #ifdef CONFIG_WASM_CLOCK
    for(uint8_t i = 0; i < 200; i++) {
        M5.update();
        if(M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {
            sync_wifi_ntp();
            break;
        }
        delay(10);
    }
    #endif

    // Test Sensor
    #ifdef CONFIG_WASM_UNITGPS
    init_uart_unitgps();
    #endif
    #ifdef CONFIG_WASM_3DCUBE_IMU6886
    init_bluetooth_serial();
    init_i2c_imu6886();
    #endif

    // Test WebAssembly
    #ifdef CONFIG_WASM_CLOCK
    if(init_wasm_clockenv() == ESP_OK) enable_wasm = true;
    #endif
    #ifdef CONFIG_WASM_3DCUBE
    if(init_wasm_3dcube() == ESP_OK) enable_wasm = true;
    #endif
    #ifdef CONFIG_WASM_3DCUBE_IMU6886
    if(init_wasm_3dcube() == ESP_OK) enable_wasm = true;
    #endif
    #ifdef CONFIG_WASM_UNITGPS
    if(init_wasm_gpsgsv() == ESP_OK) enable_wasm = true;
    #endif
}

void loop(void)
{
    M5.update();

    // Test WebAssembly
    #ifdef CONFIG_WASM_CLOCK
    if(enable_wasm) tick_wasm_clockenv();
    delay(500);
    #endif
    #ifdef CONFIG_WASM_3DCUBE
    if(enable_wasm) {
        // uint32_t time = millis();
        tick_wasm_3dcube();
        // ESP_LOGI(TAG, "time: %d", (uint32_t)(millis() - time));
        // When draw to LCD is suppressed. (calculations only)
        //     I (7264) main.cpp: time: 42
    }
    delay(1);
    #endif
    #ifdef CONFIG_WASM_3DCUBE_IMU6886
    if(enable_wasm) {
        tick_i2c_imu6886();
        tick_wasm_3dcube_imu6866();
    }
    delay(1);
    #endif
    #ifdef CONFIG_WASM_UNITGPS
    if(enable_wasm) tick_wasm_gpsgsv(true);
    delay(500);
    #endif
}
