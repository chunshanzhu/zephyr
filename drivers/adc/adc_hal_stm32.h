/*
 * Copyright (c) 2016 BayLibre, SAS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _STM32_HAL_ADC_H_
#define _STM32_HAL_ADC_H_


typedef void (*irq_config_func_t)(struct device *adc_dev);

struct adc_stm32_config {
        ADC_TypeDef *adc;
#ifdef CONFIG_ADC_STM32_INTERRUPT
        irq_config_func_t irq_config;
#endif
};

struct adc_stm32_data {
        struct k_sem sync;
	u32_t channal_group;
	u32_t result;
};

#endif  /* _STM32_HAL_ADC_H_ */
