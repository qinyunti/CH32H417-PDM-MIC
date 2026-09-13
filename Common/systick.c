#include "ch32h417.h"

uint32_t g_counter = 0;

#ifdef Core_V3F
void SysTick0_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
#else
void SysTick1_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
#endif

void systick_init(uint64_t ticks)
{
#ifdef Core_V3F
    SysTick0->ISR &= ~(1 << 0);//clear State flag
    SysTick0->CMP = ticks;
    SysTick0->CNT = 0;
    SysTick0->CTLR = 0xF;

    NVIC_SetPriority(SysTick0_IRQn, 0);
    NVIC_EnableIRQ(SysTick0_IRQn);
#else
    SysTick0->ISR &= ~(1 << 1);//clear State flag
    SysTick1->CMP = ticks;
    SysTick1->CNT = 0;
    SysTick1->CTLR = 0xF;

    NVIC_SetPriority(SysTick1_IRQn, 0);
    NVIC_EnableIRQ(SysTick1_IRQn);
#endif
}

#ifdef Core_V3F
void SysTick0_Handler(void)
{
    if(SysTick0->ISR == 1)
    {
        SysTick0->ISR &= ~(1<<0);//clear State flag
        g_counter++;
    }
}

#else

void SysTick1_Handler(void)
{
    if(SysTick0->ISR == (1<<1))
    {
        SysTick0->ISR &= ~(1<<1);//clear State flag
        g_counter++;
    }
}

#endif

uint32_t systick_get_cnt(void)
{
  return g_counter;
}

void systick_delay_ms(uint32_t ms)
{
  uint32_t t = g_counter;
  while(1){
    if((g_counter - t) >= ms){
      break;
    }
  }
}