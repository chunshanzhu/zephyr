/*
 * Copyright (c) 2015 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __INC_BOARD_H
#define __INC_BOARD_H

#include <soc.h>

/* USER push button BUT */
#define USER_PB1_GPIO_PORT	"GPIOE"
#define USER_PB1_GPIO_PIN	4

#define USER_PB2_GPIO_PORT	"GPIOE"
#define USER_PB2_GPIO_PIN	3


/* LED1 green LED */
#define LED2_GPIO_PORT	"GPIOA"
#define LED2_GPIO_PIN	6

#define LED3_GPIO_PORT  "GPIOA"
#define LED3_GPIO_PIN   7


/* Define aliases to make the basic samples work */
#define SW0_GPIO_NAME	USER_PB1_GPIO_PORT
#define SW0_GPIO_PIN	USER_PB1_GPIO_PIN
#define LED0_GPIO_PORT	LED2_GPIO_PORT
#define LED0_GPIO_PIN	LED2_GPIO_PIN

#endif /* __INC_BOARD_H */
