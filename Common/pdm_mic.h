#ifndef PDM_MIC_H
#define PDM_MIC_H

#ifdef __cplusplus
extern "C" {
#endif

void pdm_mic_init(void);  

uint32_t pdm_mic_left_get(uint8_t* buffer, uint32_t len);
uint32_t pdm_mic_left_getlen(void);
void pdm_mic_left_clr(void);

uint32_t pdm_mic_right_get(uint8_t* buffer, uint32_t len);
uint32_t pdm_mic_right_getlen(void);
void pdm_mic_rightclr(void);
void pdm_mic_poll(void);

#ifdef __cplusplus
}
#endif

#endif 
