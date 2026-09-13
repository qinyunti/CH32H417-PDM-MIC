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

static int s_mic_rec_flag = 0;
static int16_t* s_mic_rec_buffer=0;
static int s_mic_rec_total_samps=0;
static int s_mic_rec_current_samps=0;
static int s_mic_rec_chnum=2;

static void mic_rec_handle(int16_t* micleft, int16_t* micright, int samps)
{
    if(s_mic_rec_flag){
        for(int i=0; i<samps; i++){
            *s_mic_rec_buffer++ = micleft[i]; 
            *s_mic_rec_buffer++ = micright[i]; 
        }
        s_mic_rec_current_samps += samps;
        if(s_mic_rec_current_samps >= s_mic_rec_total_samps){
            s_mic_rec_flag = 0;
            xprintf("[MICREC]:len %d\r\n",44+2*s_mic_rec_total_samps*16/8);
        }
    }
}

void mic_rec_start(uint32_t addr, int samples)
{
    int chnum = 2;
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

int main(void)
{
	SystemAndCoreClockUpdate();
    systick_init(HCLKClock / 1000);
    RCC_HB2PeriphClockCmd(RCC_HB2Periph_AFIO, ENABLE);
    AFIO->PCFR1 &= ~(0x7 << 24);
    //清SW_CFG[2:0]
    AFIO->PCFR1 |= (0x4 << 24); //100:关闭SWD，PB8/PB9 作普通功能/GPIO/AF

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
    //memtester_main((ulv*)0x60000000, 0xfffff, 0x2000000, 1);
    pdm_mic_init();

	while(1)
	{
        shell_exec();
        //pdm_mic_poll();
        uint32_t leftlen = pdm_mic_left_getlen();
        uint32_t rightlen = pdm_mic_right_getlen();
        if((leftlen >= sizeof(PDM_LeftTmpBuffer)) && (rightlen >= sizeof(PDM_RightTmpBuffer))){
            /* 有足够的数据 */
            pdm_mic_left_get((uint8_t*)PDM_LeftTmpBuffer,sizeof(PDM_LeftTmpBuffer));
            pdm_mic_right_get((uint8_t*)PDM_RightTmpBuffer,sizeof(PDM_RightTmpBuffer));
            //xprintf("mic get data %d\r\n",systick_get_cnt());
            mic_rec_handle(PDM_LeftTmpBuffer,PDM_RightTmpBuffer,BufferSize);
        }
        //xprintf("1111\r\n");
        //systick_delay_ms(1000);
	}
}
