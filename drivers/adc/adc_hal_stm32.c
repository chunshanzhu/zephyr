/*
 * Copyright (c) 2016 BayLibre, SAS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define SYS_LOG_LEVEL CONFIG_SYS_LOG_SPI_LEVEL
#include <logging/sys_log.h>

#include <misc/util.h>
#include <kernel.h>
#include <board.h>
#include <errno.h>
#include <adc.h>
#include <zephyr.h>

#include <clock_control/stm32_clock_control.h>
#include <clock_control.h>

#include "adc_hal_stm32.h"

#define CONFIG_CFG(cfg)						\
((const struct adc_stm32_config * const)((cfg)->config->config_info)

#define CONFIG_DATA(cfg)					\
((struct adc_stm32_data * const)(cfg)->driver_data)

static void adc_stm32_enable(struct device *dev)
{
	const struct adc_stm32_config *config = dev->config->config_info;
	ADC_TypeDef *adc_dev = config->adc;
	ADC_HandleTypeDef hadc;
	hadc.Instance = adc_dev;
	ADC_Enable(&hadc);
}

static void adc_stm32_disable(struct device *dev)
{
	const struct adc_stm32_config *config = dev->config->config_info;
        ADC_TypeDef *adc_dev = config->adc;
        ADC_HandleTypeDef hadc;
        hadc.Instance = adc_dev;
        __HAL_ADC_DISABLE(&hadc);

}
static void adc_stm32_get_default_config(ADC_InitTypeDef *def_config)
{
	def_config->DataAlign=ADC_DATAALIGN_RIGHT;
	def_config->ScanConvMode=ADC_SCAN_DISABLE;
#if defined (CONFIG_ADC_TRIGGER_FROM_TIMER)
	def_config->ContinuousConvMode=DISABLE;
#else
	def_config->ContinuousConvMode=DISABLE;
#endif
	def_config->NbrOfConversion=1;
	def_config->DiscontinuousConvMode=DISABLE;
	def_config->NbrOfDiscConversion=0;
#if defined (CONFIG_ADC_TRIGGER_FROM_TIMER)
	def_config->ExternalTrigConv=ADC_SOFTWARE_START;
#else
	def_config->ExternalTrigConv=ADC_SOFTWARE_START;
#endif
}
static int adc_stm32_init(struct device *dev)
{
	const struct adc_stm32_config *config = dev->config->config_info;
	struct adc_stm32_data *data = dev->driver_data;
	ADC_TypeDef *adc_dev = config->adc;
	ADC_HandleTypeDef *hadc;

	ADC_InitTypeDef adc_config;
	adc_stm32_get_default_config(&adc_config);
	k_sem_init(&data->sync, 0, UINT_MAX);

	hadc->Instance = adc_dev;
	hadc->Init=adc_config;
	HAL_ADC_Init(hadc);
#ifdef CONFIG_ADC_STM32_INTERRUPT
	config->irq_config(dev);
#endif

	return 0;
}

static int adc_stm32_read(struct device *dev, struct adc_seq_table *seq_table)
{
	const struct adc_stm32_config *config = dev->config->config_info;
	struct adc_stm32_data *data = dev->driver_data;
	ADC_TypeDef *adc_dev = config->adc;
	ADC_HandleTypeDef *hadc;
	hadc->Instance = adc_dev;

	struct adc_seq_entry *entry = seq_table->entries;
	ADC_ChannelConfTypeDef channel_config;
	u32_t channel_group = 0;
	int i = 0;

	channel_config.Rank=ADC_REGULAR_RANK_1;
	channel_config.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

	for (i = 0; i < seq_table->num_entries; i++){
		if (entry->buffer_length < sizeof(data->result)) {
			return -EINVAL;
		}
		channel_config.Channel = entry->channel_id;
		HAL_ADC_ConfigChannel(hadc, &channel_config);
		HAL_ADC_Start(hadc);
		HAL_ADC_PollForConversion(hadc,10);

		data->result = HAL_ADC_GetValue(hadc);
		memcpy(entry->buffer, &data->result, sizeof(data->result));

		entry ++;
	}
	return 0;
}
static void adc_stm32_isr(struct device *dev)
{
	ARG_UNUSED(dev);
}

static const struct adc_driver_api adc_stm32_driver_api = {
	.enable = adc_stm32_enable,
	.disable = adc_stm32_disable,
	.read = adc_stm32_read,
};

#if CONFIG_ADC_0
#define CONFIG_ADC_0_BASE_ADDRESS (ADC1)
#if defined( CONFIG_ADC_STM32_INTERRUPT)
static void adc_stm32_config_func_0(struct device *dev);
#endif

static const struct adc_stm32_config adc_stm32_config_0 = {
        .adc = (ADC_TypeDef *)CONFIG_ADC_0_BASE_ADDRESS,
#if defined( CONFIG_ADC_STM32_INTERRUPT)
        .irq_config_func = adc_stm32_config_func_0,
#endif
};

static struct adc_stm32_data adc_stm32_data_0;

DEVICE_AND_API_INIT(stm32_adc_0, CONFIG_ADC_0_NAME, &adc_stm32_init,
                    &adc_stm32_data_0, &adc_stm32_config_0,
                    POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE,
                    &adc_stm32_driver_api);

#if defined( CONFIG_ADC_STM32_INTERRUPT)
static void adc_stm32_config_func_0(struct device *dev)
{
        IRQ_CONNECT(CONFIG_ADC_0_IRQ, CONFIG_ADC_0_IRQ_PRI,
                    adc_stm32_isr, DEVICE_GET(stm32_adc_0), 0);

        irq_enable(CONFIG_ADC_0_IRQ);
}
#endif
#endif /* CONFIG_ADC_0 */
