#include "ch32h417.h"
#include "pdm_mic.h"
#include "fifo.h"
#include "xprintf.h"
#include "systick.h"
#include "gpio.h"
void DMA1_Channel2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

#define BufferSize             160  /* 16k 16点数据就是1mS  160点数据就是10mS */

#define FLT_SincOrder          DFSDM_FLT_Sinc3 
#define FLT_IntegrOverSample   0
#define DataRightBitShift      2
#define SCDCntthreshold        255 

#define FLT_OverSample         128   /* 2.048M/128过采样=16K采样率*/
#define ClkoutDiv              143 /* 295M/144分频=2.0486M */

#define SAMPLE_RATE (16000)

/* limit the range of stereo data */
#define LimitData(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

/* PDM left channel data */
__attribute__((aligned(32))) int32_t PDM_LeftBuffer[BufferSize];
/* PDM right channel data */
__attribute__((aligned(32))) int32_t PDM_RightBuffer[BufferSize];
/* Left tmp data  */
__attribute__((aligned(32))) int16_t PDM_LeftTmpBuffer[BufferSize/2] = {0}; /* DMA half中断取出一半数据暂存 */
/* Right tmp data  */
__attribute__((aligned(32))) int16_t PDM_RightTmpBuffer[BufferSize/2] = {0}; /* DMA half中断取出一半数据暂存 */

static int16_t left_fifo_buffer[BufferSize*10];  
static int16_t right_fifo_buffer[BufferSize*10];

#define Alloc_Critical()    
#define Enter_Critical()    __disable_irq()
#define Exit_Critical()     __enable_irq()

static fifo_st left_fifo_dev=
{
  .in=0,
  .out=0,
  .len=0,
  .buffer_len=sizeof(left_fifo_buffer),
  .buffer=(uint8_t*)left_fifo_buffer,
};

static fifo_st right_fifo_dev=
{
  .in=0,
  .out=0,
  .len=0,
  .buffer_len=sizeof(right_fifo_buffer),
  .buffer=(uint8_t*)right_fifo_buffer,
};

uint32_t pdm_mic_left_get(uint8_t* buffer, uint32_t len)
{
  uint32_t rlen;
  Alloc_Critical();
  Enter_Critical();
  rlen = fifo_out(&left_fifo_dev, buffer, len);
  Exit_Critical();
  return rlen;
}

void pdm_mic_left_clr(void)
{
  Alloc_Critical();
  Enter_Critical();
  fifo_clr(&left_fifo_dev);
  Exit_Critical();
}

uint32_t pdm_mic_left_getlen(void)
{
  uint32_t rlen;
  Alloc_Critical();
  Enter_Critical();
  rlen = fifo_getlen(&left_fifo_dev);
  Exit_Critical();
  return rlen;
}

uint32_t pdm_mic_right_get(uint8_t* buffer, uint32_t len)
{
  uint32_t rlen;
  Alloc_Critical();
  Enter_Critical();
  rlen = fifo_out(&right_fifo_dev, buffer, len);
  Exit_Critical();
  return rlen;
}

void pdm_mic_right_clr(void)
{
  Alloc_Critical();
  Enter_Critical();
  fifo_clr(&right_fifo_dev);
  Exit_Critical();
}

uint32_t pdm_mic_right_getlen(void)
{
  uint32_t rlen;
  Alloc_Critical();
  Enter_Critical();
  rlen = fifo_getlen(&right_fifo_dev);
  Exit_Critical();
  return rlen;
}

/*
 * GPIO： PB0 = CKOUT(AF6)
 *        PB12 = DATIN1(AF6)
 */
static void pdm_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_HB2PeriphClockCmd(RCC_HB2Periph_GPIOB | RCC_HB2Periph_GPIOC | RCC_HB2Periph_GPIOE, ENABLE);

    /* Config DFSDM_CKOUT(PB0) -- DFSDM_DATIN1(PB12)  */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF6); 
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF6);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Very_High;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void dma_function_init(void) 
{
    DMA_InitTypeDef DMA_InitStructure = {0};
    RCC_HBPeriphClockCmd(RCC_HBPeriph_DMA1, ENABLE);

    /*
     * DMA1通道1 - 右声道  - 滤波器0
     * DMA1通道2  -左声道  - 滤波器1
     */
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (int32_t)& DFSDM_FLT0->RDATAR; /* 滤波器转换后的输出数据 见RM 42.6.13*/
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)PDM_RightBuffer; /* 存储空间 */
    DMA_InitStructure.DMA_BufferSize = BufferSize;     /* 存储空间大小 */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; /* 外设到存储 */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; /* 外设地址不递增 */
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;       /* 存储地址递增 */
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; /* 外设访问单位WORD */
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;  /* 存储访问单位 */
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;             /* 回环模式 */
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  /* 优先级高 */
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  /* 非存储到存储传输 */
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_MuxChannelConfig(DMA_MuxChannel1, 107);  /* DMA请求通道 107 DFSDM_DMA0 见手册P150 表 10-2 DMA 复用器输入到资源的分配表 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    DMA_DeInit(DMA1_Channel2);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (int32_t)& DFSDM_FLT1->RDATAR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)PDM_LeftBuffer;
    DMA_InitStructure.DMA_BufferSize = BufferSize;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);  
    DMA_MuxChannelConfig(DMA_MuxChannel2, 108);   /* DMA请求通道 108 DFSDM_DMA1 见手册P150 表 10-2 DMA 复用器输入到资源的分配表 */
    DMA_Cmd(DMA1_Channel2, ENABLE);

    NVIC_SetPriority(DMA1_Channel1_IRQn,1);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    NVIC_SetPriority(DMA1_Channel2_IRQn,1);
    NVIC_EnableIRQ(DMA1_Channel2_IRQn);

    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); /* 传输完成中断使能 */
    DMA_ITConfig(DMA1_Channel1, DMA_IT_HT, ENABLE); /* 传输一半中断使能 */
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TE, ENABLE); /* 传输错误中断使能 */
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE); /* 传输完成中断使能 */
    DMA_ITConfig(DMA1_Channel2, DMA_IT_HT, ENABLE); /* 传输一半中断使能 */
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TE, ENABLE); /* 传输错误中断使能 */
}

static int left_over_run = 0;
static int left_over_run_print_flag = 0;
static int right_over_run = 0;
static int right_over_run_print_flag = 0;

void DMA1_Channel2_IRQHandler(void)
{
    uint32_t i;
    uint32_t wlen;
    //uint32_t intreg = DMA1->INTFR;
    //xprintf("dma1_ch2 %d %x\r\n",systick_get_cnt(),intreg);
    //gpio_toggle(1);
    if(DMA_GetITStatus(DMA1, DMA1_IT_HT2) == SET){
        /* get the PCM stereo data , half中断数据位于前半段 */
        for(i = 0; i < BufferSize / 2; i++) {
            PDM_LeftTmpBuffer[i] = LimitData((PDM_LeftBuffer[i] >> 8), -32768, 32767);
        }
        wlen = fifo_in(&left_fifo_dev, (uint8_t*)PDM_LeftTmpBuffer, sizeof(PDM_LeftTmpBuffer)); /* 中断中调用无需临界段保护 */
        if(wlen < sizeof(PDM_LeftTmpBuffer)){
            if(left_over_run_print_flag == 0){
                xprintf("left in overrun\r\n");
                left_over_run_print_flag=1;
                left_over_run++;
            }
        }else{
            left_over_run_print_flag = 0;
        }
        //DMA_ClearFlag(DMA1, DMA1_FLAG_HT2);
        DMA_ClearITPendingBit(DMA1, DMA1_IT_HT2);
    }
    if(DMA_GetITStatus(DMA1, DMA1_IT_TC2) == SET){
        /* get the PCM stereo data , 完成中断数据位于后半段 */
        for(i = BufferSize / 2; i < BufferSize; i++) {
            PDM_LeftTmpBuffer[i- BufferSize / 2] = LimitData((PDM_LeftBuffer[i] >> 8), -32768, 32767);
        }
        wlen =fifo_in(&left_fifo_dev, (uint8_t*)PDM_LeftTmpBuffer, sizeof(PDM_LeftTmpBuffer)); /* 中断中调用无需临界段保护 */
        if(wlen < sizeof(PDM_LeftTmpBuffer)){
            if(left_over_run_print_flag == 0){
                xprintf("left in overrun\r\n");
                left_over_run_print_flag=1;
                left_over_run++;
            }
        }else{
            left_over_run_print_flag = 0;
        }
        //DMA_ClearFlag(DMA1, DMA1_FLAG_TC2);
        DMA_ClearITPendingBit(DMA1, DMA1_IT_TC2);
    }
    if(DMA_GetITStatus(DMA1, DMA1_IT_TE2) == SET){
        //DMA_ClearFlag(DMA1, DMA1_FLAG_TE2);
        DMA_ClearITPendingBit(DMA1, DMA1_IT_TE2);
    }
    ///DMA_ClearITPendingBit(DMA1, DMA1_IT_GL2);
}

void DMA1_Channel1_IRQHandler(void)
{
    uint32_t i;
    uint32_t wlen;
    //uint32_t intreg = DMA1->INTFR;
    //xprintf("dma1_ch1 %d %x\r\n",systick_get_cnt(),intreg);
    //gpio_toggle(1);
    if(DMA_GetITStatus(DMA1, DMA1_IT_HT1) == SET){
        /* get the PCM stereo data , half中断数据位于前半段 */
        for(i = 0; i < BufferSize / 2; i++) {
            PDM_RightTmpBuffer[i] = LimitData((PDM_RightBuffer[i] >> 8), -32768, 32767);
        }
        wlen =fifo_in(&right_fifo_dev, (uint8_t*)PDM_RightTmpBuffer, sizeof(PDM_RightTmpBuffer)); /* 中断中调用无需临界段保护 */
        if(wlen < sizeof(PDM_RightTmpBuffer)){
            if(right_over_run_print_flag == 0){
                xprintf("right in overrun\r\n");
                right_over_run_print_flag=1;
                right_over_run++;
            }
        }else{
            right_over_run_print_flag = 0;
        }
        //DMA_ClearFlag(DMA1, DMA1_FLAG_HT1);
        DMA_ClearITPendingBit(DMA1, DMA1_IT_HT1);
    }
    if(DMA_GetITStatus(DMA1, DMA1_IT_TC1) == SET){
        /* get the PCM stereo data , 完成中断数Right段 */
        for(i = BufferSize / 2; i < BufferSize; i++) {
            PDM_RightTmpBuffer[i- BufferSize / 2] = LimitData((PDM_RightBuffer[i] >> 8), -32768, 32767);
        }
        wlen =fifo_in(&right_fifo_dev, (uint8_t*)PDM_RightTmpBuffer, sizeof(PDM_RightTmpBuffer)); /* 中断中调用无需临界段保护 */
        if(wlen < sizeof(PDM_RightTmpBuffer)){
            if(right_over_run_print_flag == 0){
                xprintf("right in overrun");
                right_over_run_print_flag=1;
                right_over_run++;
            }
        }else{
            right_over_run_print_flag = 0;
        }
        //DMA_ClearFlag(DMA1, DMA1_FLAG_TC1);
        DMA_ClearITPendingBit(DMA1, DMA1_IT_TC1);
    }
    if(DMA_GetITStatus(DMA1, DMA1_IT_TE1) == SET){
        //DMA_ClearFlag(DMA1, DMA1_FLAG_TE1);
        DMA_ClearITPendingBit(DMA1, DMA1_IT_TE1);
    }
    ///DMA_ClearITPendingBit(DMA1, DMA1_IT_GL1);
}

static void dfsdm_function_init(void)
{
    DFSDM_ChannelInitTypeDef DFSDM_ChannelInitStructure = {0};
    DFSDM_FilterInitTypeDef DFSDM_FilterInitStructure = {0}; 
    DFSDM_RcInitTypeDef DFSDM_RcInitStructure = {0};

	RCC_HB2PeriphClockCmd(RCC_HB2Periph_DFSDM, ENABLE);

    /* Configure output serial clock source and divider */
    DFSDM_OutSerialClkConfig(DFSDM_AudioClk, ClkoutDiv);

    /* initialize the parameters of DFSDM */
    DFSDM_ChannelStructInit(&DFSDM_ChannelInitStructure);
    DFSDM_FilterStructInit(&DFSDM_FilterInitStructure);
    DFSDM_RcStructInit(&DFSDM_RcInitStructure);

    /* initialize DFSDM channel 0 */
    DFSDM_ChannelInitStructure.DFSDM_ChAWDSincFilterOrder = DFSDM_AWD_FastSinc;
    DFSDM_ChannelInitStructure.DFSDM_ChAWDFilterOverSample = DFSDM_AWD_FLT_Bypass;
    DFSDM_ChannelInitStructure.DFSDM_ChSPIClockSource = DFSDM_InternalClkOut;
    DFSDM_ChannelInitStructure.DFSDM_ChSerialInterface = DFSDM_SPIFalling;  /* CLK下降沿采样 */
    DFSDM_ChannelInitStructure.DFSDM_ChCalibrationOffset = 0;
    DFSDM_ChannelInitStructure.DFSDM_ChDataPackMode = DFSDM_StandardMode;
    DFSDM_ChannelInitStructure.DFSDM_ChDataMultiplexer = DFSDM_SerialInput; /* 00：通道 y 输入数据取自外部串行输入，为 1 位
值。即来自DFSDM_DATINx引脚 */
    DFSDM_ChannelInitStructure.DFSDM_ChInPinSelect = DFSDM_SelectNext; /* 通道输入取自下一通道（通道（y+1）取 2 的模）的引脚 这里通道为0所以选择引脚DATIN1 */
    DFSDM_ChannelInitStructure.DFSDM_ChShortCircuitDetMode = ENABLE;
    DFSDM_ChannelInitStructure.DFSDM_ChSCDCntthreshold = SCDCntthreshold;
    DFSDM_ChannelInitStructure.DFSDM_ChDataRightBitShift = DataRightBitShift;
    DFSDM_ChannelInit(DFSDM_Channel0, &DFSDM_ChannelInitStructure);  

    /* initialize DFSDM channel 1 */
    DFSDM_ChannelInitStructure.DFSDM_ChAWDSincFilterOrder = DFSDM_AWD_FastSinc;
    DFSDM_ChannelInitStructure.DFSDM_ChAWDFilterOverSample = DFSDM_AWD_FLT_Bypass;
    DFSDM_ChannelInitStructure.DFSDM_ChSPIClockSource = DFSDM_InternalClkOut;
    DFSDM_ChannelInitStructure.DFSDM_ChSerialInterface = DFSDM_SPIRising;  /* CLK上升沿采样 */
    DFSDM_ChannelInitStructure.DFSDM_ChInPinSelect = DFSDM_SelectCurrent; /* 0：通道输入取自同一通道 y 的引脚。 这里通道所以选择DATIN1引脚 */
    DFSDM_ChannelInitStructure.DFSDM_ChSCDCntthreshold = SCDCntthreshold; 
    DFSDM_ChannelInitStructure.DFSDM_ChDataRightBitShift = DataRightBitShift; 
    DFSDM_ChannelInit(DFSDM_Channel1, &DFSDM_ChannelInitStructure);  

    /* initialize DFSDM filter 0 and filter 1 */ 
    DFSDM_FilterInitStructure.DFSDM_FltSincOrder = FLT_SincOrder;
    DFSDM_FilterInitStructure.DFSDM_FltOverSample = FLT_OverSample;
    DFSDM_FilterInitStructure.DFSDM_FltIntegratorOverSample = FLT_IntegrOverSample;
    DFSDM_FilterInit(DFSDM_FLT0, &DFSDM_FilterInitStructure);
    DFSDM_FilterInit(DFSDM_FLT1, &DFSDM_FilterInitStructure);  

    /* initialize DFSDM filter 0 regular conversions */
    DFSDM_RcInitStructure.DFSDM_RcChannel = DFSDM_RC_Channel0;
    DFSDM_RcInitStructure.DFSDM_RcContinuousMode = ENABLE;
    DFSDM_RcInitStructure.DFSDM_RcFastMode = ENABLE;
    DFSDM_RcInitStructure.DFSDM_RcDMAMode = ENABLE;
    DFSDM_RcInit(DFSDM_FLT0, &DFSDM_RcInitStructure);

    /* initialize DFSDM filter 1 regular conversions */
    DFSDM_RcInitStructure.DFSDM_RcChannel = DFSDM_RC_Channel1;
    DFSDM_RcInitStructure.DFSDM_RcContinuousMode = ENABLE;
    DFSDM_RcInitStructure.DFSDM_RcFastMode = ENABLE;
    DFSDM_RcInitStructure.DFSDM_RcDMAMode = ENABLE;
    DFSDM_RcInit(DFSDM_FLT1, &DFSDM_RcInitStructure);
    
    /* enable DFSDM channel 0 and channel 1*/
    DFSDM_ChannelCmd(DFSDM_Channel0, ENABLE);
    DFSDM_ChannelCmd(DFSDM_Channel1, ENABLE);

    /* enable DFSDM filter 0 and filter 1 */
    DFSDM_FilterCmd(DFSDM_FLT0, ENABLE);
    DFSDM_FilterCmd(DFSDM_FLT1, ENABLE);

    /* enable DFSDM interface */
    DFSDM_Cmd(ENABLE);
    //DFSDM_RcContinuousCmd(DFSDM_FLT0,ENABLE);
    //DFSDM_RcContinuousCmd(DFSDM_FLT1,ENABLE);

    /* enable regular channel conversion by software */
    DFSDM_RcSoftStartConversion(DFSDM_FLT0);
    DFSDM_RcSoftStartConversion(DFSDM_FLT1);

    //DFSDM_RcDMACmd(DFSDM_FLT0,ENABLE);
    //DFSDM_RcDMACmd(DFSDM_FLT1,ENABLE);
}

void pdm_mic_init(void)
{
    pdm_gpio_init();
    dma_function_init();
    dfsdm_function_init();
}

void pdm_mic_poll(void)
{
	uint32_t i = 0;

        /* wait for DMA half-full transmit complete */
        while(RESET == DMA_GetFlagStatus(DMA1, DMA1_FLAG_HT1));
        while(RESET == DMA_GetFlagStatus(DMA1, DMA1_FLAG_HT2));

        /* get the PCM stereo data */
        for(i = 0; i < BufferSize / 2; i++) {
            //PDM_PlayBuffer[2 * i] = LimitData((PDM_LeftBuffer[i] >> 8), -32768, 32767);
            //PDM_PlayBuffer[(2 * i) + 1] = LimitData((PDM_RightBuffer[i] >> 8), -32768, 32767);
        }
        /* clear the half transfer finish flag */
        DMA_ClearFlag(DMA1, DMA1_FLAG_HT1);
        DMA_ClearFlag(DMA1, DMA1_FLAG_HT2);
                xprintf("hc\r\n");

        /* wait for DMA full transmit complete */
        while(RESET == DMA_GetFlagStatus(DMA1, DMA1_FLAG_TC1));
        while(RESET == DMA_GetFlagStatus(DMA1, DMA1_FLAG_TC2));

        /* get the PCM stereo data */
        for(i = BufferSize / 2; i < BufferSize; i++) {
            //PDM_PlayBuffer[2 * i] = LimitData((PDM_LeftBuffer[i] >> 8), -32768, 32767);
            //PDM_PlayBuffer[(2 * i) + 1] = LimitData((PDM_RightBuffer[i] >> 8), -32768, 32767);
        }
        /* clear the full transfer finish flag */
        DMA_ClearFlag(DMA1, DMA1_FLAG_TC1);
        DMA_ClearFlag(DMA1, DMA1_FLAG_TC2);   
                        xprintf("tc\r\n");     
}