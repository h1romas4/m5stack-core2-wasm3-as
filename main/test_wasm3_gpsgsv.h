#include <stdint.h>

void as_gc_unpin_ptr(uint32_t wasm_prt);
void as_gc_collect(void);
esp_err_t init_wasm_gpsgsv(void);
esp_err_t tick_wasm_gpsgsv(bool clear);
