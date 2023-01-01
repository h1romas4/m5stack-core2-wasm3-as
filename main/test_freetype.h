#include "ffsupport.h"
#include "font_render.h"

void load_ttf_spiffs_psram(void);
void load_ttf_spiffs(void);
font_render_t create_freetype_render(uint32_t font_size, uint32_t font_cache_size);
void draw_freetype_string(const char *string, int32_t poX, int32_t poY, uint16_t fg, font_render_t *render);
