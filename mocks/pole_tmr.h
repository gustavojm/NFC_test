#include <stdint.h>

void pole_tmr_init(void);

int32_t pole_tmr_set_freq(uint32_t tick_rate_hz);

void pole_tmr_start(void);

void pole_tmr_stop(void);

uint32_t pole_tmr_started(void);
