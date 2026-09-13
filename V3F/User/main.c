/********************************** (C) COPYRIGHT *******************************
 * File Name          : main_v3f.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2025/07/17
 * Description        : Main program body for V3F.
 *********************************************************************************
 * Copyright (c) 2025 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
   This example is used to demonstrate the FMC interface operating SDRAM-W9825G6KH.
   Pin:
   SDCLK -----PF2
   SDCKE0-----PC5
   SDNE0 -----PC2
   BA0   -----PB14
   BA1   -----PB15
   NRAS  -----PF11
   NCAS  -----PF12
   SDNWE -----PC0
   DQM0  -----PC12
   DQM1  -----PC11
   A0    -----PF5
   A1    -----PB3
   A2    -----PB4
   A3    -----PB8
   A4    -----PB9
   A5    -----PB6
   A6    -----PA10
   A7    -----PA11
   A8    -----PA12
   A9    -----PA13
   A10   -----PB10
   A11   -----PB11
   A12   -----PB12
   D0    -----PD14
   D1    -----PD15
   D2    -----PD0
   D3    -----PD1
   D4    -----PE7
   D5    -----PE8
   D6    -----PE9
   D7    -----PE10
   D8    -----PE11
   D9    -----PE12
   D10   -----PE13
   D11   -----PE14
   D12   -----PE15
   D13   -----PD8
   D14   -----PD9
   D15   -----PD10
*/

#include "hardware.h"
#include "xprintf.h"
#include "shell.h"
#include "shell_func.h"
#include "uart.h"
#include "sdram.h"
#include "systick.h"
#include "memtester.h"
#include "ch32h417.h"

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

int main(void)
{
	SystemInit();
	SystemAndCoreClockUpdate();

    systick_init(HCLKClock / 1000);
    systick_delay_ms(5000);  /* 延迟避免以上来SWCLK切换为串口无法连接仿真器 */

    NVIC_WakeUp_V5F(Core_V5F_StartAddr);
    while(1);

    RCC_HB2PeriphClockCmd(RCC_HB2Periph_AFIO, ENABLE);
    AFIO->PCFR1 &= ~(0x7 << 24);
    //清SW_CFG[2:0]
    AFIO->PCFR1 |= (0x4 << 24); //100:关闭SWD，PB8/PB9 作普通功能/GPIO/AF

    uart_init(0, 115200);
    shell_set_itf(shell_read_cb, shell_write_cb, (shell_cmd_cfg*)g_shell_cmd_list_ast, 1);
    xdev_out(xprintf_out_port);
    xdev_in(xprintf_in_port);

	xprintf("SystemClk:%d\r\n", SystemClock);
	xprintf("V3F SystemCoreClk:%d\r\n", SystemCoreClock);

    RCC_HB1PeriphClockCmd(RCC_HB1Periph_PWR, ENABLE);
    PWR_VIO18ModeCfg(PWR_VIO18CFGMODE_SW);
    PWR_VIO18LevelCfg(PWR_VIO18Level_MODE3);
    sdram_init();
    memtester_main((ulv*)0x60000000, 0xfffff, 0x2000000, 1);

	while(1)
	{
        shell_exec();
	}
}
