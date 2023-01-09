#include "M5Core2.h"
#include "esp_log.h"
#include "Preferences.h"
#include "WiFi.h"

static const char *TAG = "test_nvs_wifi.cpp";

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
