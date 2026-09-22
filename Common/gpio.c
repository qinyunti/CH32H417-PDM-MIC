#include "ch32h417.h"

void gpio_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure={0};

  RCC_HB2PeriphClockCmd(RCC_HB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Very_High;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void gpio_set(int id, int level)
{
  GPIO_WriteBit(GPIOA, GPIO_Pin_2, (level != 0) ? Bit_SET : Bit_RESET);
}

void gpio_toggle(int id)
{
  GPIO_WriteBit(GPIOA, GPIO_Pin_2, GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2)^0x01);
} 