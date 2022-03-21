#include "M5Core2.h"
#include "Preferences.h"
#include "WiFi.h"

#include "test_freetype.h"
#include "test_wasm3.h"

static const char *TAG = "main.cpp";

/**
 * FreeType member
 */
font_render_t font_render;

/**
 * Wasm3 member
 */
boolean enable_wasm = false;

void sync_wifi_ntp(void)
{
    Preferences preferences;

    if(!preferences.begin("wifi", true)) return;

    String ssid = preferences.getString("ssid");
    String passwd = preferences.getString("passwd");

    ESP_LOGI(TAG, "Connect to %s", ssid.c_str());
    WiFi.begin(ssid.c_str(), passwd.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
    }
    ESP_LOGI(TAG, "Connected!");
    // Sync NTP
    configTime(9 * 3600L, 0, "ntp1.jst.mfeed.ad.jp", "ntp2.jst.mfeed.ad.jp", "ntp3.jst.mfeed.ad.jp");
    // Wait Time Sync
    struct tm timeInfo;
    while(true) {
        getLocalTime(&timeInfo);
        if(timeInfo.tm_year > 0) {
            break;
        }
        delay(500);
    }
    ESP_LOGI(TAG, "Configured time from NTP");
    WiFi.disconnect();
    // not enough memory..
    ESP_LOGI(TAG, "Restart ESP32...");
    // reboot..
    esp_restart();
}

void setup(void)
{
    M5.begin();

    // Test FreeType
    init_freetype();
    font_render = create_freetype_render(/* font size */ 40, /* font cache */ 24);
    draw_freetype_string("M5Stack Core2", 10, 10 + 50, M5.Lcd.color565(0, 0, 255), &font_render);
    draw_freetype_string("Development", 10, 10 + 50 * 2, M5.Lcd.color565(255, 255, 255), &font_render);
    draw_freetype_string("Board", 10, 10 + 50 * 3, M5.Lcd.color565(255, 255, 255), &font_render);
    draw_freetype_string("Xtensa", 10, 10 + 50 * 4, M5.Lcd.color565(255, 0, 0), &font_render);

    // Test NVS and Wifi
    for(uint8_t i = 0; i < 200; i++) {
        M5.update();
        if(M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {
            sync_wifi_ntp();
            break;
        }
        delay(10);
    }

    // Test WebAssembly
    if(init_wasm() == ESP_OK) enable_wasm = true;
}

void loop(void)
{
    M5.update();
    // Test WebAssembly
    if(enable_wasm) tick_wasm();
    delay(500);
}
