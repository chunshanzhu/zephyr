/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>

void main(void)
{
	u32_t count = 0;
	while(1)
	{
		count += 1;
		printk("Hello World! %s %s board the %dth print\n", CONFIG_ARCH,CONFIG_BOARD,count);
		k_sleep(1000);
	}
}
