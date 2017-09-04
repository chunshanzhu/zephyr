/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include "stm32_adc.h"

void main(void)
{
	Adc_Init();
	u32_t count = 0;
	volatile u16_t adc_num;
	while(1)
	{
		adc_num = Get_Adc(1);
		count += 1;
		printk("Hello World! %s %s board the %dth print\n", CONFIG_ARCH,CONFIG_BOARD,count);
		printk("the number readed from adc1 channel 1 is :%d \n",adc_num);
		k_sleep(1000);
	}
}
