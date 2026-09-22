#ifndef ADC_ECHO_H
#define ADC_ECHO_H

#ifdef __cplusplus
extern "C" {
#endif

void adc_echo_init(void);  
uint32_t adc_echo_get(uint8_t* buffer, uint32_t len);
uint32_t adc_echo_getlen(void);
void adc_echo_clr(void);

#ifdef __cplusplus
}
#endif

#endif 
