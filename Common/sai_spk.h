#ifndef SAI_SPK_H
#define SAI_SPK_H

#ifdef __cplusplus
extern "C" {
#endif

void sai_spk_init(void);  
uint32_t sai_spk_put(uint8_t* buffer, uint32_t len);
uint32_t sai_spk_getfree(void);
void sai_spk_clr(void);

#define SPK_CH_NUM 2
#define SPK_SAMPLE_RATE (16000)
#define SPK_OneShotSamps             160  /* 一次DMA传输的采样点数, 16k 16点数据就是1mS  160点数据就是10mS */

#ifdef __cplusplus
}
#endif

#endif 
