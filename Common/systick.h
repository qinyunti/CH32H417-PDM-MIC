#ifndef SYSTICK_H
#define SYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void systick_init(uint64_t ticks);
uint32_t systick_get_cnt(void);
void systick_delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif

