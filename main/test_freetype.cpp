#include "M5Core2.h"
#include "SPIFFS.h"
#include "ffsupport.h"
#include "font_render.h"
#include "freetype_helper.h"

#include "test_freetype.h"

static const char *TAG = "test_freetype.cpp";

/**
 * SPIFFS member
 */
fs::SPIFFSFS SPIFFS_FONT;

/**
 * FreeType member
 */
font_face_t font_face;
const uint8_t alphamap[16] = {0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255};

/**
 * Utility draw_freetype_bitmap
 */
void draw_freetype_bitmap(int32_t cx, int32_t cy, uint16_t bw, uint16_t bh, uint16_t fg, uint8_t *bitmap)
{
    uint32_t pos = 0;
    uint16_t bg = 0;
    M5.Lcd.startWrite();
    for (int y = 0; y < bh; y++) {
        for (int x = 0; x < bw; x++) {
            if (pos & 0x1) {
                M5.Lcd.drawPixel(cx + x, cy + y, M5.Lcd.alphaBlend(alphamap[bitmap[pos >> 1] & 0x0F], fg, bg));
            } else {
                M5.Lcd.drawPixel(cx + x, cy + y, M5.Lcd.alphaBlend(alphamap[bitmap[pos >> 1] >> 4], fg, bg));
            }
            pos++;
        }
    }
    M5.Lcd.endWrite();
}

/**
 * Utility drawString
 */
void draw_freetype_string(const char *string, int32_t poX, int32_t poY, uint16_t fg, font_render_t *render)
{
    int16_t sumX = 0;
    uint16_t len = strlen(string);
    uint16_t n = 0;
    uint16_t base_y = poY;

    while (n < len) {
        uint16_t uniCode = decodeUTF8((uint8_t *)string, &n, len - n);
        if (font_render_glyph(render, uniCode) != ESP_OK) {
            ESP_LOGE(TAG, "Font render faild.");
        }
        draw_freetype_bitmap(poX + render->bitmap_left,
            base_y - render->bitmap_top,
            render->bitmap_width,
            render->bitmap_height,
            fg,
            render->bitmap
        );
        poX += render->advance;
        sumX += render->advance;
    }
}

/**
 * load_ttf_spiffs_psram (fast)
 */
void load_ttf_spiffs_psram(void)
{
    // FreeType initialize
    SPIFFS_FONT.begin(false, "/font", 4, "font");

    // Read fonts from SPIFFS and extract them in PSRAM. (fast)
    File fp = SPIFFS_FONT.open("/GENSHINM.TTF", "r");
    size_t font_size = fp.size();
    uint8_t *font_data = (uint8_t *)heap_caps_malloc(font_size, MALLOC_CAP_SPIRAM);
    fp.read(font_data, font_size);
    if (font_face_init(&font_face, font_data, font_size) != ESP_OK) {
        ESP_LOGE(TAG, "Font load faild.");
    }
    fp.close();

    SPIFFS_FONT.end();

    ESP_LOGI(TAG, "Minimum free heap size: %d bytes", esp_get_minimum_free_heap_size());
}

/**
 * load_ttf_spiffs (slow)
 */
void load_ttf_spiffs(void)
{
    // FreeType initialize
    SPIFFS_FONT.begin(false, "/font", 4, "font");
    ffsupport_setffs(SPIFFS_FONT);
    if (font_face_init_fs(&font_face, "/GENSHINM.TTF") != ESP_OK) {
        ESP_LOGE(TAG, "Font load faild.");
    }
}

font_render_t create_freetype_render(uint32_t font_size, uint32_t font_cache_size)
{
    font_render_t font_render;

    // Font render initialize
    if (font_render_init(&font_render, &font_face, font_size, font_cache_size) != ESP_OK) {
        ESP_LOGE(TAG, "Render creation failed.");
    }

    return font_render;
}
