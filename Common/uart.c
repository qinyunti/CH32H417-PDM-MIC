#include "ch32h417.h"
#include "fifo.h"

void USART6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

static uint8_t rx_fifo_buffer[1024];
static uint8_t tx_fifo_buffer[1024];

#define Alloc_Critical()    
#define Enter_Critical()    __disable_irq()
#define Exit_Critical()     __enable_irq()

fifo_st rx_fifo_dev=
{
  .in=0,
  .out=0,
  .len=0,
  .buffer_len=sizeof(rx_fifo_buffer),
  .buffer=rx_fifo_buffer
};

fifo_st tx_fifo_dev=
{
  .in=0,
  .out=0,
  .len=0,
  .buffer_len=sizeof(tx_fifo_buffer),
  .buffer=tx_fifo_buffer
};

void uart_init(int id, uint32_t baudrate)
{
    if(id == 0){
        GPIO_InitTypeDef  GPIO_InitStructure = {0};
        USART_InitTypeDef USART_InitStructure = {0};

        RCC_HB2PeriphClockCmd(RCC_HB2Periph_AFIO | RCC_HB2Periph_GPIOA | RCC_HB2Periph_GPIOB, ENABLE);
        RCC_HB1PeriphClockCmd(RCC_HB1Periph_USART6, ENABLE);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF8);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF8);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Very_High;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Very_High;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        USART_InitStructure.USART_BaudRate = baudrate;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

        USART_Init(USART6, &USART_InitStructure);
        USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
	      NVIC_EnableIRQ(USART6_IRQn);
        USART_Cmd(USART6, ENABLE);
    }
}

void USART6_IRQHandler(void)
{
    if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
        uint8_t ch = USART_ReceiveData(USART6);
        fifo_in(&rx_fifo_dev, &ch, 1);
    }
}

uint32_t uart_read(int id, uint8_t* buffer, uint32_t len)
{
#if 1
  uint32_t rlen;
  Alloc_Critical();
  Enter_Critical();
  rlen = fifo_out(&rx_fifo_dev, buffer, len);
  Exit_Critical();
  return rlen;
#else 
  if(USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == SET){
    buffer[0] = USART_ReceiveData(USART6);
    return 1;
  } else {
    return 0;
  }
#endif
}

void uart_send(int id, uint8_t* buffer, uint32_t len)
{
    for(uint32_t i=0; i<len; i++){
        USART_ClearFlag(USART6, USART_FLAG_TC);
        USART_SendData(USART6, buffer[i]);
        while(USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET) /* waiting for sending finish */{
        }
    }
}


