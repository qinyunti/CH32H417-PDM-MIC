#include "ch32h417.h"
#include "sai_spk.h"
#include "fifo.h"
#include "xprintf.h"
#include "systick.h"
#include "math.h"



void DMA1_Channel3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));


#define PlayBufferShots          10   /* 播放缓存区可以存的DMA传输笔数 */

/* SPK channel data */
static __attribute__((aligned(32)))  int16_t SAI_Data[SPK_OneShotSamps*SPK_CH_NUM];  /* DMA发送缓存 CH_NUM个通道 */

static int16_t fifo_buffer[SPK_OneShotSamps*SPK_CH_NUM*PlayBufferShots];  /* 播放缓存 DMA发送缓存的PlayBufferShots倍数  */

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

uint32_t sai_spk_put(uint8_t* buffer, uint32_t len)
{
  uint32_t wlen
  Alloc_Critical();
  Enter_Critical();
  wlen = fifo_in(&fifo_dev, buffer, len);
  Exit_Critical();
  return wlen;
}

void sai_spk_clr(void)
{
  Alloc_Critical();
  Enter_Critical();
  fifo_clr(&fifo_dev);
  Exit_Critical();
}

uint32_t sai_spk_getfree(void)
{
  uint32_t len;
  Alloc_Critical();
  Enter_Critical();
  len = fifo_getfree(&fifo_dev);
  Exit_Critical();
  return len;
}

static void sai_spk_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_HB2PeriphClockCmd(RCC_HB2Periph_GPIOB | RCC_HB2Periph_GPIOC | RCC_HB2Periph_GPIOE, ENABLE);

    // SAI_FS_A PC3(AF7)
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF7);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // SAI_SCK_A PC2(AF7)
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF7);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // SAI_SD_A PB2(AF6)
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource2, GPIO_AF6);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // SAI_MCLK_A PE2(AF6)
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF6);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

static void sai_config(uint32_t SampleRate)
{
    SAI_InitTypeDef      SAI_InitStructure      = {0};
    SAI_FrameInitTypeDef SAI_FrameInitStructure = {0};
    SAI_SlotInitTypeDef  SAI_SlotInitStructure  = {0};

    RCC_HB2PeriphClockCmd(RCC_HB2Periph_SAI, ENABLE);

    /* The SAI Clock configuration is calculated as follows:
    SAI_CK_x  = SysClk / 6
    MCLK_x = SAI_CK_x / MCKDIV[5:0] with MCLK_x = 256 * FS
    FS = SAI_CK_x / (MCKDIV[5:0] * 256)
    MCKDIV[5:0] = (SysClk / 6) / (FS * 256) */

    RCC_ClocksTypeDef RCC_ClocksStatus = {0};
    RCC_GetClocksFreq(&RCC_ClocksStatus);

    const uint32_t tmpdiv = (RCC_ClocksStatus.SYSCLK_Frequency / 6) / (SampleRate * 256);
    /* 如果sysclk=400M
     * div = 400000000/6/16000/256 = 16
     * 所以实际MCLK=400000000/6/16=4.167M
     * LRCK=4.167M/256=16.276K
     * SCK=32*16.276K=520.833K
     * 如果sysclk=295M
     * div = 295000000/6/16000/256 = 12
     * 所以实际MCLK=295000000/6/12=4.097M
     * LRCK=4.097M/256=16.005K
     * SCK=32*16.005K=512.153K
     * SCK = MCLK*（FRL[7:0]+1）/256
     */
    xprintf("sai in clk:%x, div:%d\r\n",RCC_ClocksStatus.SYSCLK_Frequency, tmpdiv);
    SAI_InitStructure.SAI_NoDivider     = SAI_MasterDivider_Enabled;
    SAI_InitStructure.SAI_MasterDivider = tmpdiv;
    SAI_InitStructure.SAI_AudioMode     = SAI_Mode_MasterTx;
    SAI_InitStructure.SAI_Protocol      = SAI_Free_Protocol;
    SAI_InitStructure.SAI_DataSize      = SAI_DataSize_16b;
    SAI_InitStructure.SAI_FirstBit      = SAI_FirstBit_MSB;  /* 高位在前 */
    SAI_InitStructure.SAI_ClockStrobing = SAI_ClockStrobing_RisingEdge; /* 上升沿采样 */
    SAI_InitStructure.SAI_Synchro       = SAI_Asynchronous;
    SAI_InitStructure.SAI_FIFOThreshold = SAI_FIFOThreshold_HalfFull;

    SAI_Init(SAI_Block_A, &SAI_InitStructure);

    SAI_FrameInitStructure.SAI_FrameLength       = 32;
    SAI_FrameInitStructure.SAI_ActiveFrameLength = 16;
    SAI_FrameInitStructure.SAI_FSDefinition      = SAI_FS_StartFrame; /* FS边沿为帧开始SOF */
    /** FS上升沿表示帧开始
     */
    SAI_FrameInitStructure.SAI_FSPolarity        = SAI_FS_ActiveHigh;
    /* 见MS4344 手册的P9 MSB对应的CLK前面还有一个空的CLK 
     * 所以FSOFF=1 在 Slot 0 第一位的前一位上使能FS；
     */
    SAI_FrameInitStructure.SAI_FSOffset          = SAI_FS_BeforeFirstBit;
    SAI_FrameInit(SAI_Block_A, &SAI_FrameInitStructure);

    SAI_SlotInitStructure.SAI_FirstBitOffset = 0;  /* 指定开始位的偏移 */
    SAI_SlotInitStructure.SAI_SlotSize       = SAI_SlotSize_16b; /* 每通道数据位 */
    SAI_SlotInitStructure.SAI_SlotNumber     = 2;  /* 2通道 */
    SAI_SlotInitStructure.SAI_SlotActive     = SAI_SlotActive_ALL; /* 所有通道有效 */
    SAI_SlotInit(SAI_Block_A, &SAI_SlotInitStructure);

    SAI_FlushFIFO(SAI_Block_A);
}

void dma_function_init(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t pAddr, uint32_t mAddr,
                 uint16_t Length)
{

    DMA_InitTypeDef DMA_InitStructure = {0};
    RCC_HBPeriphClockCmd(RCC_HBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMAy_Channelx);
    DMA_InitStructure.DMA_PeripheralBaseAddr = pAddr;
    DMA_InitStructure.DMA_Memory0BaseAddr    = mAddr;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize         = Length;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
    DMA_Init(DMAy_Channelx, &DMA_InitStructure);

    NVIC_SetPriority(DMA1_Channel3_IRQn,1);
    NVIC_EnableIRQ(DMA1_Channel3_IRQn);

    DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE); /* 传输完成中断使能 */
    DMA_ITConfig(DMA1_Channel3, DMA_IT_HT, ENABLE); /* 传输一半中断使能 */
    DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, ENABLE); /* 传输错误中断使能 */
}

static int over_run = 1;  /* 默认认为是SPK缓存中没有数据 */
static int over_run_print_flag = 0;

void DMA1_Channel3_IRQHandler(void)
{
    ///uint32_t intreg = DMA1->INTFR;
    ///xprintf("dma1_ch3 %d %x\r\n",systick_get_cnt(),intreg);
    if(DMA_GetITStatus(DMA1, DMA1_IT_HT3) == SET){
        ///xprintf("ht:%d\r\n",systick_get_cnt());
        /* half中断数据前半段已经发送完 写后半部 */
        /**
         * 最开始必须继续2笔以上数据
         */
        int needsize;
        if(over_run){
            needsize = sizeof(SAI_Data)/2;
        }else{
            needsize = (sizeof(SAI_Data)/2)*2;
        }
        uint32_t len = fifo_getlen(&fifo_dev);
        if(len < needsize){
           over_run = 1;
           /* 数据不够写0 */ 
           memset( (uint8_t*)(&(SAI_Data[SPK_OneShotSamps*SPK_CH_NUM/2])),0,sizeof(SAI_Data)/2);
           if(over_run_print_flag==0){
            over_run_print_flag=1; /* 无溢出到溢出 只打印一次*/
            xprintf("spk read fifo down over %d\r\n",systick_get_cnt());
           }
        }else{
            over_run = 0;
            over_run_print_flag=0;
            /* 数据够 从FIFO中取出数据 发送 */
            fifo_out(&fifo_dev, (uint8_t*)(&(SAI_Data[SPK_OneShotSamps*SPK_CH_NUM/2])), sizeof(SAI_Data)/2); /* 中断中调用无需临界段保护 */
        }
        DMA_ClearITPendingBit(DMA1, DMA1_IT_HT3);
    }
    if(DMA_GetITStatus(DMA1, DMA1_IT_TC3) == SET){
        ///xprintf("tc:%d\r\n",systick_get_cnt());
        /* 完成中断数据后半段已经发送完 写前半部 */
        /**
         * 最开始必须继续2笔以上数据
         */
        int needsize;
        if(over_run){
            needsize = sizeof(SAI_Data)/2;
        }else{
            needsize = (sizeof(SAI_Data)/2)*2;
        }
        uint32_t len = fifo_getlen(&fifo_dev);
        if(len < needsize){
           over_run = 1;
           /* 数据不够写0 */ 
           memset( (uint8_t*)(&(SAI_Data[0])),0,sizeof(SAI_Data)/2);
           if(over_run_print_flag==0){
            over_run_print_flag=1; /* 无溢出到溢出 只打印一次*/
            xprintf("spk read fifo down over %d\r\n",systick_get_cnt());
           }
        }else{
            over_run=0;
            over_run_print_flag=0;
            /* 数据够 从FIFO中取出数据 发送 */
            fifo_out(&fifo_dev, (uint8_t*)(&(SAI_Data[0])), sizeof(SAI_Data)/2); /* 中断中调用无需临界段保护 */
        }
        DMA_ClearITPendingBit(DMA1, DMA1_IT_TC3);
    }
    if(DMA_GetITStatus(DMA1, DMA1_IT_TE3) == SET){
        xprintf("te:%d\r\n",systick_get_cnt());
        DMA_ClearITPendingBit(DMA1, DMA1_IT_TE3);
    }
}

void sai_spk_init(void)
{
    sai_spk_gpio_init();
    sai_config(SPK_SAMPLE_RATE);
    memset(SAI_Data,0,sizeof(SAI_Data));

#if 0
#define PI          3.1415926535f
#define TAU         (2.0f * PI)
    for (int i = 0; i < SPK_OneShotSamps; i += 2)
    {
        float t         = ((float)i) / ((float)SPK_SAMPLE_RATE);
        SAI_Data[i]     = 1000.0f * sinf(440.0f * TAU * t);
        SAI_Data[i + 1] = SAI_Data[i];
    }
#endif

    dma_function_init(DMA1_Channel3, (uint32_t)(&SAI_Block_A->DATAR), (uint32_t)SAI_Data, sizeof(SAI_Data)/sizeof(int16_t));  /* 注意长度单位是数据宽度 */

     /* DMA请求通道 108 DFSDM_DMA1 见手册P150 表 10-2 DMA 复用器输入到资源的分配表 */
    DMA_MuxChannelConfig(DMA_MuxChannel3, 112); /* 112 SAI_A_TX */

    DMA_Cmd(DMA1_Channel3, ENABLE);
    SAI_DMACmd(SAI_Block_A, ENABLE);
    
    SAI_Cmd(SAI_Block_A, ENABLE);
    xprintf("sai_spk_init done\r\n");
}