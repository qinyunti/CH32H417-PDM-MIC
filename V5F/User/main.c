#include "hardware.h"
#include "xprintf.h"
#include "shell.h"
#include "shell_func.h"
#include "uart.h"
#include "sdram.h"
#include "systick.h"
#include "memtester.h"
#include "ch32h417.h"
#include "pdm_mic.h"
#include "sai_spk.h"
#include "wav.h"
#include <stdio.h>
#include "adc_echo.h"
#include "gpio.h"

static int s_mic_rec_flag = 0;
static int16_t* s_mic_rec_buffer=0;
static int s_mic_rec_total_samps=0;
static int s_mic_rec_current_samps=0;
static int s_mic_rec_chnum=2;

static int s_spk_play_flag = 0;
static int16_t* s_spk_play_buffer=0;
static int s_spk_play_total_samps=0;
static int s_spk_play_current_samps=0;
static int s_spk_data_chnum=2;

static void mic_rec_handle(int16_t* micleft, int16_t* micright, int16_t* echo, int samps)
{
    if(s_mic_rec_flag){
        for(int i=0; i<samps; i++){
            *s_mic_rec_buffer++ = micleft[i]; 
            *s_mic_rec_buffer++ = micright[i]; 
            *s_mic_rec_buffer++ = echo[i]; 
        }
        s_mic_rec_current_samps += samps;
        if(s_mic_rec_current_samps >= s_mic_rec_total_samps){
            s_mic_rec_flag = 0;
            xprintf("[MICREC]:len %d\r\n",44+3*s_mic_rec_total_samps*16/8);
        }
    }
}

static __attribute__((aligned(32)))  s16 SAI_Data[SPK_OneShotSamps*SPK_CH_NUM];  /* 一次操作一笔DMA发送缓存大小 */

static void spk_play_handle(void)
{
    uint32_t togetsamps=0; /* 本次可以发送的采样点数 */
    s16* p = SAI_Data;
    if(s_spk_play_flag){
        if(sai_spk_getfree() >= sizeof(SAI_Data)) {
            /* 有剩余空间可写ONT_SHOT_SAMPS点数 再操作一笔 */
            
            /* 最多一次操作ONT_SHOT_SAMPS点数,最后不够ONT_SHOT_SAMPS点数则有多少发多少 */
            togetsamps = s_spk_play_total_samps - s_spk_play_current_samps;
            if(togetsamps > SPK_OneShotSamps) {
                togetsamps = SPK_OneShotSamps; 
            }
            ///xprintf("[SPKPLAY]:playsamps:%d\r\n",s_spk_play_current_samps);
            for(int i=0; i<togetsamps; i++){
                if(s_spk_data_chnum >= SPK_CH_NUM)
                {
                    /* 原始数据通道数可能大于播放通道数 只播放前面PLAY_CHNUM通道  */
                    for(int i=0; i<SPK_CH_NUM; i++){
                        *p++ = *s_spk_play_buffer++; 
                    }
                    for(int i=SPK_CH_NUM; i<s_spk_data_chnum; i++){
                        s_spk_play_buffer++; /* 多的原始数据丢掉 */
                    }
                } else{
                    /* 原始数据通道数可能小于播放通道数 无数据的通道填写0 */
                    for(int i=0; i<s_spk_data_chnum; i++){
                        *p++ = *s_spk_play_buffer++; 
                    }
                    for(int i=s_spk_data_chnum; i<SPK_CH_NUM; i++){
                        *p++ = 0;          /* 多的播放通道填写0 */
                    }   
                }
            }
            sai_spk_put((uint8_t*)SAI_Data, sizeof(SAI_Data));
        }

        s_spk_play_current_samps += togetsamps;
        if(s_spk_play_current_samps >= s_spk_play_total_samps){
            s_spk_play_flag = 0;
            xprintf("[SPKPLAY]:done %d\r\n",s_spk_play_total_samps);
        }
    }
}

/**
 * @brief 
 * 
 * @param addr 存储wav文件的地址
 */
void spk_play_start(uint32_t addr)
{
    wav_t wav;
    int res = wav_decode((uint8_t*)addr, &wav);
    if(res < 0){
      xprintf("no wav\r\n");
      return;
    } else {
      xprintf("get wav data:\r\n");
      xprintf("off:%d\r\n",wav.off); 
      xprintf("chunksize:%d\r\n",wav.chunksize); 
      xprintf("audioformat:%d\r\n",wav.audioformat); 
      xprintf("numchannels:%d\r\n",wav.numchannels); 
      xprintf("samplerate:%d\r\n",wav.samplerate); 
      xprintf("byterate:%d\r\n",wav.byterate); 
      xprintf("blockalign:%d\r\n",wav.blockalign); 
      xprintf("bitspersample:%d\r\n",wav.bitspersample); 
      xprintf("datasize:%d\r\n",wav.datasize); 

      s_spk_play_buffer = (int16_t*)(addr + wav.off);
      s_spk_play_total_samps=wav.datasize/wav.blockalign;
      s_spk_play_current_samps=0;
      s_spk_data_chnum=wav.numchannels;
      s_spk_play_flag = 1;
    }
}

void mic_rec_start(uint32_t addr, int samples)
{
    int chnum = 3;
    int freq = 16000;
    s_mic_rec_buffer = (int16_t*)(addr+44);
    s_mic_rec_total_samps = samples;
    s_mic_rec_current_samps = 0;
    s_mic_rec_chnum = chnum;
    /*
     * 添加wav头信息
     */
    uint32_t chunksize = 44-8+samples*chnum*16/8;
    uint8_t* p = (uint8_t*)addr;
    uint32_t bps = freq*chnum*16/8;
    uint32_t datalen = samples*chnum*16/8;
    p[0] = 'R';
    p[1] = 'I';
    p[2] = 'F';
    p[3] = 'F';
    p[4] = chunksize & 0xFF;
    p[5] = (chunksize>>8) & 0xFF;
    p[6] = (chunksize>>16) & 0xFF;
    p[7] = (chunksize>>24) & 0xFF;
    p[8] = 'W';
    p[9] = 'A';
    p[10] = 'V';
    p[11] = 'E';

    p[12] = 'f';
    p[13] = 'm';
    p[14] = 't';
    p[15] = ' ';

    p[16] = 16;  /* Subchunk1Size */
    p[17] = 0;
    p[18] = 0;
    p[19] = 0;

    p[20] = 1;  /* PCM */
    p[21] = 0;

    p[22] = chnum; /* 通道数 */
    p[23] = 0;

    p[24] = freq & 0xFF;
    p[25] = (freq>>8) & 0xFF;
    p[26] = (freq>>16) & 0xFF;
    p[27] = (freq>>24) & 0xFF; 

    p[28] = bps & 0xFF;      /* ByteRate */
    p[29] = (bps>>8) & 0xFF;
    p[30] = (bps>>16) & 0xFF;
    p[31] = (bps>>24) & 0xFF; 

    p[32] = chnum*16/8; /* BlockAlign */
    p[33] = 0;

    p[34] = 16;  /* BitsPerSample */
    p[35] = 0;

    p[36] = 'd';
    p[37] = 'a';
    p[38] = 't';
    p[39] = 'a';

    p[40] = datalen & 0xFF;
    p[41] = (datalen>>8) & 0xFF;
    p[42] = (datalen>>16) & 0xFF;
    p[43] = (datalen>>24) & 0xFF; 

    s_mic_rec_flag = 1;
}

static void xprintf_out_port(int ch)
{
    uint8_t val = ch;
    uart_send(0, &val,1);
}

static int xprintf_in_port(void)
{
    uint32_t len;
    uint8_t val;
    do
    {
        len = uart_read(0, &val,1);
    }while(len == 0);
    return val;
}

uint32_t shell_read_cb(uint8_t *buff, uint32_t len)
{
    return uart_read(0, buff, len);
}

void shell_write_cb(uint8_t *buff, uint32_t len)
{
    uart_send(0, buff, len);
}
#define BufferSize 160

/* Left tmp data  */
static __attribute__((aligned(32))) int16_t PDM_LeftTmpBuffer[BufferSize] = {0}; 
/* Right tmp data  */
static __attribute__((aligned(32))) int16_t PDM_RightTmpBuffer[BufferSize] = {0}; 
/* Echo tmp data  */
static __attribute__((aligned(32))) int16_t ECHO_TmpBuffer[BufferSize] = {0}; 


int main(void)
{
	SystemAndCoreClockUpdate();
    systick_init(HCLKClock / 1000);
    RCC_HB2PeriphClockCmd(RCC_HB2Periph_AFIO, ENABLE);
    AFIO->PCFR1 &= ~(0x7 << 24);
    //清SW_CFG[2:0]
    AFIO->PCFR1 |= (0x4 << 24); //100:关闭SWD，PB8/PB9 作普通功能/GPIO/AF
    gpio_init();
    uart_init(0, 500000);
    shell_set_itf(shell_read_cb, shell_write_cb, (shell_cmd_cfg*)g_shell_cmd_list_ast, 1);
    xdev_out(xprintf_out_port);
    xdev_in(xprintf_in_port);

	xprintf("SystemClk:%d\r\n", SystemClock);
	xprintf("V5F SystemCoreClk:%d\r\n", SystemCoreClock);
    xprintf("reset:%x\r\n",RCC->RSTSCKR);
    RCC_HB1PeriphClockCmd(RCC_HB1Periph_PWR, ENABLE);
    PWR_VIO18ModeCfg(PWR_VIO18CFGMODE_SW);
    PWR_VIO18LevelCfg(PWR_VIO18Level_MODE3);
    sdram_init();
    ///memtester_main((ulv*)0x60000000, 0xfffff, 0x2000000, 1);
    pdm_mic_init();
    sai_spk_init();  
    adc_echo_init();
	while(1)
	{
        shell_exec();
        //pdm_mic_poll();
        uint32_t leftlen = pdm_mic_left_getlen();
        uint32_t rightlen = pdm_mic_right_getlen();
        uint32_t echolen = adc_echo_getlen();

        static int echo_over_run = 1;  /* 默认认为是SPK缓存中没有数据 */
        static int left_over_run = 1;  /* 默认认为是SPK缓存中没有数据 */
        static int right_over_run = 1;  /* 默认认为是SPK缓存中没有数据 */

        int needsize;
        if(echo_over_run){
            needsize = sizeof(ECHO_TmpBuffer);
        }else{
            needsize = sizeof(ECHO_TmpBuffer)*2;
        }
        if(echolen < needsize){
           echo_over_run = 1;
           /* 数据不够写0 */ 
           //memset( (uint8_t*)ECHO_TmpBuffer,0,sizeof(ECHO_TmpBuffer));
        }else{
            echo_over_run = 0;
            /* 数据够 从FIFO中取出数据 */
            //adc_echo_get((uint8_t*)ECHO_TmpBuffer,sizeof(ECHO_TmpBuffer));
        }

        if(left_over_run){
            needsize = sizeof(PDM_LeftTmpBuffer);
        }else{
            needsize = sizeof(PDM_LeftTmpBuffer)*2;
        }
        if(leftlen < needsize){
           left_over_run = 1;
           /* 数据不够写0 */ 
           //memset( (uint8_t*)PDM_LeftTmpBuffer,0,sizeof(PDM_LeftTmpBuffer));
        }else{
            left_over_run = 0;
            /* 数据够 从FIFO中取出数据 */
            //pdm_mic_left_get((uint8_t*)PDM_LeftTmpBuffer,sizeof(PDM_LeftTmpBuffer));
        }

        if(right_over_run){
            needsize = sizeof(PDM_RightTmpBuffer);
        }else{
            needsize = sizeof(PDM_RightTmpBuffer)*2;
        }
        if(rightlen < needsize){
           right_over_run = 1;
           /* 数据不够写0 */ 
           //memset( (uint8_t*)PDM_RightTmpBuffer,0,sizeof(PDM_RightTmpBuffer));
        }else{
            right_over_run = 0;
            /* 数据够 从FIFO中取出数据 */
            //pdm_mic_right_get((uint8_t*)PDM_RightTmpBuffer,sizeof(PDM_RightTmpBuffer));
        }

        //xprintf("mic get data %d\r\n",systick_get_cnt());
        if((left_over_run == 0) && (right_over_run == 0) && (echo_over_run == 0)){
            ///xprintf("data handle %d %d\r\n", echolen, systick_get_cnt());
            pdm_mic_left_get((uint8_t*)PDM_LeftTmpBuffer,sizeof(PDM_LeftTmpBuffer));
            pdm_mic_right_get((uint8_t*)PDM_RightTmpBuffer,sizeof(PDM_RightTmpBuffer));
            adc_echo_get((uint8_t*)ECHO_TmpBuffer,sizeof(ECHO_TmpBuffer));
            mic_rec_handle(PDM_LeftTmpBuffer,PDM_RightTmpBuffer,ECHO_TmpBuffer,BufferSize);
        }
        spk_play_handle();
        //xprintf("1111\r\n");
        //systick_delay_ms(1000);
	}
}
