#include <stdint.h>

void as_gc_unpin_ptr(uint32_t wasm_prt);
void as_gc_collect(void);
esp_err_t init_wasm_3dcube(void);
esp_err_t tick_wasm_3dcube(void);
esp_err_t tick_wasm_3dcube_imu6866(void);
