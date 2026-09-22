#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

void gpio_init(void);  
void gpio_set(int id, int level);  
void gpio_toggle(int id);  

#ifdef __cplusplus
}
#endif

#endif 
