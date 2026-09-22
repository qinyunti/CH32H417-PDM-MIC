#include "ch32h417.h"
#include "adc_echo.h"
#include "fifo.h"
#include "xprintf.h"
#include "systick.h"
#include "math.h"
#include "gpio.h"

void DMA1_Channel4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

#define EchoBufferShots          10   /* 接收缓存区可以存的DMA传输笔数 */
#define Echo_OneShotSamps        160  /* 一笔DMA接收的点数 */

/* ECho channel data */
static __attribute__((aligned(32)))  int16_t Echo_Data[Echo_OneShotSamps];  /* DMA接收缓存 */

static int16_t fifo_buffer[Echo_OneShotSamps*EchoBufferShots];  /* 接收缓存 DMA接收缓存的EchoBufferShots倍数  */

#define Alloc_Critical()    
#define Enter_Critical()    __disable_irq()
#define Exit_Critical()     __enable_irq()

static fifo_st fifo_dev=
{
  .in=0,
  .out=0,
  .len=0,
  .buffer_len=sizeof(fifo_buffer),
  .buffer=(uint8_t*)fifo_buffer,
};

uint32_t adc_echo_get(uint8_t* buffer, uint32_t len)
{
  uint32_t rlen;
  //xprintf("out:%d\r\n",len);
  Alloc_Critical();
  Enter_Critical();
  rlen = fifo_out(&fifo_dev, buffer, len);
  Exit_Critical();
  return rlen;
}

void adc_echo_clr(void)
{
  Alloc_Critical();
  Enter_Critical();
  fifo_clr(&fifo_dev);
  Exit_Critical();
}

uint32_t adc_echo_getlen(void)
{
  uint32_t len;
  Alloc_Critical();
  Enter_Critical();
  len = fifo_getlen(&fifo_dev);
  Exit_Critical();
  return len;
}

static void adc_echo_gpio_init(void)
{
  /* PC0 HSADC0 ADC10 */
  GPIO_InitTypeDef GPIO_InitStructure = {0};
	RCC_HB2PeriphClockCmd(RCC_HB2Periph_GPIOC, ENABLE );

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init( GPIOC, &GPIO_InitStructure );
}

void  adc_echo_function_init(void)
{
  ADC_InitTypeDef ADC_InitStructure={0};

  RCC_HB2PeriphClockCmd(RCC_HB2Periph_ADC1, ENABLE );
	RCC_ADCCLKConfig(RCC_ADCCLKSource_HCLK);
	RCC_ADCHCLKCLKAsSourceConfig(RCC_PPRE2_DIV2,RCC_HCLK_ADCPRE_DIV8);
 
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
  ADC_LowPowerModeCmd(ADC1,DISABLE); 

  ///ADC_SMP_ModeConfig(ADC1,ADC_Channel_10,ADC_SMP_CFG_MODE1);
  ADC_Cmd(ADC1, ENABLE);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_BufferCmd(ADC1, ENABLE);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_CyclesMode5);
  ADC_ExternalTrigConvCmd(ADC1, ENABLE);

  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
}

void  adc_echo_dma_init(uint32_t memadr, uint16_t bufsize)
{
  DMA_InitTypeDef DMA_InitStructure = {0};

  RCC_HBPeriphClockCmd(RCC_HBPeriph_DMA1, ENABLE);

  DMA_DeInit(DMA1_Channel4);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (int32_t)& ADC1->RDATAR;
  DMA_InitStructure.DMA_Memory0BaseAddr = memadr;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = bufsize;  /* 单位为数据宽度 */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel4, &DMA_InitStructure); 

  DMA_MuxChannelConfig(DMA_MuxChannel4, 0x78); /* 120 ADC1 手册P154 */
  DMA_Cmd(DMA1_Channel4, ENABLE);  

  NVIC_SetPriority(DMA1_Channel4_IRQn,1);
  NVIC_EnableIRQ(DMA1_Channel4_IRQn);

  DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE); /* 传输完成中断使能 */
  DMA_ITConfig(DMA1_Channel4, DMA_IT_HT, ENABLE); /* 传输一半中断使能 */
  DMA_ITConfig(DMA1_Channel4, DMA_IT_TE, ENABLE); /* 传输错误中断使能 */
}

void adc_echo_tim_init(void)
{
  //TIM_OCInitTypeDef       TIM_OCInitStructure = {0};
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

  RCC_HB1PeriphClockCmd(RCC_HB1Periph_TIM3,ENABLE);

  /* 147.5M/16K=9218.75*/
  TIM_TimeBaseInitStructure.TIM_Period = 9215; /* 9217 100.01HZ  9216 100.02HZ  9215 100.03HZ */
  TIM_TimeBaseInitStructure.TIM_Prescaler = 0;   /* 定时器时钟不分频 */
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

  //TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //TIM_OCInitStructure.TIM_Pulse = 9218/2;
  //TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  //TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  //TIM_CtrlPWMOutputs(TIM3, ENABLE);
  //TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
  TIM_ARRPreloadConfig(TIM3, ENABLE);  /* 自动重载值寄存器 */
  TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);
  TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
  TIM_Cmd(TIM3, ENABLE);
}

void adc_echo_init(void)
{
  adc_echo_gpio_init();
  adc_echo_function_init();
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_CyclesMode5);
  adc_echo_dma_init((uint32_t)Echo_Data, Echo_OneShotSamps);
  adc_echo_tim_init();
	///ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

static int over_run_print_flag=0;
static int over_run=0;
void DMA1_Channel4_IRQHandler(void)
{
    uint32_t wlen;
    //uint32_t intreg = DMA1->INTFR;
    //xprintf("dma1_ch4 %d %x\r\n",systick_get_cnt(),intreg);
    //gpio_toggle(1);
    if(DMA_GetITStatus(DMA1, DMA1_IT_HT4) == SET){
        /* half中断数据位于前半段 */
        wlen = fifo_in(&fifo_dev, (uint8_t*)Echo_Data, sizeof(Echo_Data)/2); /* 中断中调用无需临界段保护 */
        //xprintf("in:%d\r\n",wlen);
        if(wlen < sizeof(Echo_Data)/2){
            if(over_run_print_flag == 0){
                xprintf("echo in overrun %d\r\n",systick_get_cnt());
                over_run_print_flag=1;
                over_run++;
            }
        }else{
            over_run_print_flag = 0;
        }
        //DMA_ClearFlag(DMA1, DMA1_FLAG_HT4);
        DMA_ClearITPendingBit(DMA1, DMA1_IT_HT4);
    }
    if(DMA_GetITStatus(DMA1, DMA1_IT_TC4) == SET){
        /* 完成中断数据位于后半段 */
        wlen =fifo_in(&fifo_dev, (uint8_t*)(&(Echo_Data[Echo_OneShotSamps/2])), sizeof(Echo_Data)/2); /* 中断中调用无需临界段保护 */
        //xprintf("in:%d\r\n",wlen);
        if(wlen < sizeof(Echo_Data)/2){
            if(over_run_print_flag == 0){
                xprintf("echo in overrun %d\r\n",systick_get_cnt());
                over_run_print_flag=1;
                over_run++;
            }
        }else{
            over_run_print_flag = 0;
        }
        //DMA_ClearFlag(DMA1, DMA1_FLAG_TC4);
        DMA_ClearITPendingBit(DMA1, DMA1_IT_TC4);
    }
    if(DMA_GetITStatus(DMA1, DMA1_IT_TE4) == SET){
        //DMA_ClearFlag(DMA1, DMA1_FLAG_TE4);
        DMA_ClearITPendingBit(DMA1, DMA1_IT_TE4);
    }
    ///DMA_ClearITPendingBit(DMA1, DMA1_IT_GL4);
}