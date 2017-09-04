#ifndef __STM32_ADC_H
#include <zephyr.h>
#include <soc.h>
#include <board.h>
#include <zephyr/types.h>
#define ADCx_CHANNELa	ADC_CHANNEL_4
#define ADCx	ADC1
#define	RANGE_12BITS	((uint32_t) 4095)
void Adc_Init(void);
u16_t	Get_Adc(u32_t ch);
u16_t	Get_Adc_Average(u8_t ch,u8_t times);
#endif
