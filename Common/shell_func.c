#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "shell.h"
#include "shell_func.h"
#include "xmodem.h"
#include "xprintf.h"
#include "uart.h"
#include "systick.h"

static void helpfunc(uint8_t* param);
static void rxmemfunc(uint8_t* param);
static void sxmemfunc(uint8_t* param);

static void printmemfunc(uint8_t* param);
static void setmemfunc(uint8_t* param);

static void micrecfunc(uint8_t* param);
static void spkplayfunc(uint8_t* param);

/**
 * 最后一行必须为0,用于结束判断
*/
const shell_cmd_cfg g_shell_cmd_list_ast[ ] = 
{
  { (uint8_t*)"help",         helpfunc,         (uint8_t*)"help"}, 
  { (uint8_t*)"rxmem",        rxmemfunc,        (uint8_t*)"rxmem addr[hex] len"}, 
  { (uint8_t*)"sxmem",        sxmemfunc,        (uint8_t*)"sxmem addr[hex] len"}, 
  { (uint8_t*)"printmem",     printmemfunc,     (uint8_t*)"printmem mode[hex/dec] addr[hex] len datasize[8/16/32] sig[1/0]"}, 
  { (uint8_t*)"setmem",       setmemfunc,       (uint8_t*)"setmem addr[hex] val[hex]"}, 
  { (uint8_t*)"micrec",       micrecfunc,       (uint8_t*)"micrec addr[HEX] samps"}, 
  { (uint8_t*)"spkplay",      spkplayfunc,      (uint8_t*)"spkplay addr[HEX]"}, 
  { (uint8_t*)0,		          0 ,               0},
};

static void helpfunc(uint8_t* param)
{
	  (void)param;
    unsigned int i;
    xprintf("\r\n");
    xprintf("**************\r\n");
    xprintf("*   SHELL    *\r\n");
    xprintf("*   V1.0     *\r\n");
    xprintf("**************\r\n");
    xprintf("\r\n");
    for (i=0; g_shell_cmd_list_ast[i].name != 0; i++)
    {
        xprintf("%02d.",i);
        xprintf("%-16s",g_shell_cmd_list_ast[i].name);
        xprintf("%s\r\n",g_shell_cmd_list_ast[i].helpstr);
    }
}

static uint8_t rxtx_buf[1029];

static uint32_t getms(void)
{
  return systick_get_cnt();
}

static uint32_t io_read(uint8_t* buffer, uint32_t len)
{
  return uart_read(0, buffer, len);
}

static void io_read_flush(void)
{
  uint8_t tmp;
  while(0 != uart_read(0, &tmp, 1));
}

static uint32_t io_write(uint8_t* buffer, uint32_t len)
{
  uart_send(0, buffer, len);
  return len;
}

static uint32_t mem_read(uint32_t addr, uint8_t* buffer, uint32_t len)
{
  memcpy(buffer, (uint8_t*)addr, len);
  return len;
}

static uint32_t mem_write(uint32_t addr, uint8_t* buffer, uint32_t len)
{
  memcpy((uint8_t*)addr, buffer, len);
  return len;
}

static void rxmemfunc(uint8_t* param)
{
  uint32_t addr;
  uint32_t len;
  int res = 0;
  if(2 == sscanf((const char*)param, "%*s %lx %ld", &addr, &len))
  {
    xprintf("rxmem to 0x%x %d\r\n",addr,len);
    xmodem_cfg_st cfg=
      {
        .buffer = rxtx_buf,
        .crccheck = 1,
        .getms = getms,
        .io_read = io_read,
        .io_read_flush = io_read_flush,
        .io_write = io_write,
        .start_timeout = 60,
        .packet_timeout = 1000,
        .ack_timeout = 1000,
        .mem_write = mem_write,
        .addr = addr,
        .totallen = len,
      };
      xmodem_init_rx(&cfg);
      while((res = xmodem_rx()) == 0);
      xprintf("res:%d\r\n",res);
  }
}

static void sxmemfunc(uint8_t* param)
{
  uint32_t addr;
  uint32_t len;
  int res = 0;
  if(2 == sscanf((const char*)param, "%*s %lx %ld", &addr, &len))
  {
    xprintf("sxmem to 0x%x %d\r\n",addr,len);
    xmodem_cfg_st cfg=
    {
      .buffer = rxtx_buf,
      .plen = 1024,
      .getms = getms,
      .io_read = io_read,
      .io_read_flush = io_read_flush,
      .io_write = io_write,
      .start_timeout = 60,
      .packet_timeout = 1000,
      .ack_timeout = 5000,
      .mem_read = mem_read,
      .addr = addr,
      .totallen = len,
    };
    xmodem_init_tx(&cfg);
    while((res = xmodem_tx()) == 0);
    xprintf("res:%d\r\n",res);
  }
}

static void setmemfunc(uint8_t* param)
{
  uint32_t addr;
  uint32_t val;
  if(2 == sscanf((const char*)param, "%*s %lx %lx", &addr, &val))
  {
    xprintf("setmem  %x %x\r\n",addr,val);
    if((addr % 4) ==0)
    {
      *(volatile uint32_t*)addr = val;
      xprintf("%x\r\n",*(volatile uint32_t*)addr);
    }
    else
    {
      xprintf("addr must be mul of 4\r\n");
    }
  }
}

static void printmemfunc(uint8_t* param)
{
  uint32_t addr;
  uint32_t len;
  uint8_t mode[64];
  int datasize;
  uint8_t* tmp8_u;
  uint16_t* tmp16_u;
  uint32_t* tmp32_u;
  int8_t* tmp8_i;
  int16_t* tmp16_i;
  int32_t* tmp32_i;

  int sig;
  if(5 == sscanf((const char*)param, "%*s %s %lx %ld %d %d", mode, &addr, &len, &datasize, &sig))
  {
    if(strncmp((const char*)mode,"hex", 3) == 0)
    {
      if(datasize == 8)
      {
        tmp8_u = (uint8_t*)addr;
        for(uint32_t i=0; i<len ;i++)
        {
          if(i%16 == 0)
          {
            xprintf("\r\n[%08x]:",addr+i*1);
          }
          xprintf("%02x ",tmp8_u[i]);
        }
        xprintf("\r\n");
      }
      else if(datasize == 16)
      {
        tmp16_u = (uint16_t*)addr;
        for(uint32_t i=0; i<len ;i++)
        {
          if(i%16 == 0)
          {
            xprintf("\r\n[%08x]:",addr+i*2);
          }
          xprintf("%04x ",tmp16_u[i]);
        }
        xprintf("\r\n");
      }
      else if(datasize == 32)
      {
        tmp32_u = (uint32_t*)addr;
        for(uint32_t i=0; i<len ;i++)
        {
          if(i%16 == 0)
          {
            xprintf("\r\n[%08x]:",addr+i*4);
          }
          xprintf("%08x ",tmp32_u[i]);
        }
        xprintf("\r\n");
      }
      else
      {
        xprintf("datasize must be 8/16/32\r\n");
      }
    }
    else if(strncmp((const char*)mode,"dec", 3) == 0)
    {
      if(datasize == 8)
      {
        if(sig == 0)
        {
          tmp8_u = (uint8_t*)addr;
          for(uint32_t i=0; i<len ;i++)
          {
            if(i%16 == 0)
            {
              xprintf("\r\n");
            }
            xprintf("%d ",tmp8_u[i]);
          }
          xprintf("\r\n");
        }
        else
        {
          tmp8_i = (int8_t*)addr;
          for(uint32_t i=0; i<len ;i++)
          {
            if(i%16 == 0)
            {
              xprintf("\r\n");
            }
            xprintf("%d ",tmp8_i[i]);
          }
          xprintf("\r\n");
        }
      }
      else if(datasize == 16)
      {
        if(sig == 0)
        {
          tmp16_u = (uint16_t*)addr;
          for(uint32_t i=0; i<len ;i++)
          {
            if(i%16 == 0)
            {
              xprintf("\r\n");
            }
            xprintf("%d ",tmp16_u[i]);
          }
          xprintf("\r\n");
        }
        else
        {
          tmp16_i = (int16_t*)addr;
          for(uint32_t i=0; i<len ;i++)
          {
            if(i%16 == 0)
            {
              xprintf("\r\n");
            }
            xprintf("%d ",tmp16_i[i]);
          }
          xprintf("\r\n");
        }
      }
      else if(datasize == 32)
      {
        if(sig == 0)
        {
          tmp32_u = (uint32_t*)addr;
          for(uint32_t i=0; i<len ;i++)
          {
            if(i%16 == 0)
            {
              xprintf("\r\n");
            }
            xprintf("%d ",tmp32_u[i]);
          }
          xprintf("\r\n");
        }
        else
        {
          tmp32_i = (int32_t*)addr;
          for(uint32_t i=0; i<len ;i++)
          {
            if(i%16 == 0)
            {
              xprintf("\r\n");
            }
            xprintf("%d ",tmp32_i[i]);
          }
          xprintf("\r\n");
        }
      }
      else
      {
        xprintf("datasize must be 8/16/32\r\n");
      }
    }
    else
    {
      xprintf("mode must be [hex/dec]\r\n");
    }
  }
}

extern void mic_rec_start(uint32_t addr, int samples);
static void micrecfunc(uint8_t* param)
{
    uint32_t addr;
    int samples;
    if(2 == sscanf((const char*)param, "%*s %lx %d", &addr, &samples))
    {
      xprintf("[MIC]:%x,samples:%d\r\n",addr,samples);
      mic_rec_start(addr, samples);
    }
    else
    {
      xprintf("param err\r\n");
    }
}


extern void spk_play_start(uint32_t addr);
static void spkplayfunc(uint8_t* param)
{
    uint32_t addr;
    if(1 == sscanf((const char*)param, "%*s %lx", &addr))
    {
      spk_play_start(addr);
      xprintf("[SPK]: start %x\r\n",addr);
    }
    else
    {
      xprintf("param err\r\n");
    }
}
