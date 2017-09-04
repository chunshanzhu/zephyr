#include <zephyr.h>
#include <misc/printk.h>
#include "stm32_adc.h"

ADC_HandleTypeDef    AdcHandle;
void Adc_Init(void)
{
  ADC_ChannelConfTypeDef   sConfig;
  ADC_AnalogWDGConfTypeDef AnalogWDGConfig;

  /* Configuration of ADCx init structure: ADC parameters and regular group */
  AdcHandle.Instance = ADCx;

  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DISABLE;              /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
#if defined ADC_TRIGGER_FROM_TIMER
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
#else
  AdcHandle.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode to have maximum conversion speed (no delay between conversions) */
#endif
  AdcHandle.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
#if defined ADC_TRIGGER_FROM_TIMER
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_Tx_TRGO;  /* Trig of conversion start done by external event */
#else
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
#endif

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
     printk("adc init failed \n");
  }

  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
  /* Note: Considering IT occurring after each ADC conversion if ADC          */
  /*       conversion is out of the analog watchdog window selected (ADC IT   */
  /*       enabled), select sampling time and ADC clock with sufficient       */
  /*       duration to not create an overhead situation in IRQHandler.        */
  sConfig.Channel      = ADCx_CHANNELa;
  sConfig.Rank         = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;

  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    printk("adc channel config failed \n");
  }

  /* Set analog watchdog thresholds in order to be between steps of DAC       */
  /* voltage.                                                                 */
  /*  - High threshold: between DAC steps 1/2 and 3/4 of full range:          */
  /*                    5/8 of full range (4095 <=> Vdda=3.3V): 2559<=> 2.06V */
  /*  - Low threshold:  between DAC steps 0 and 1/4 of full range:            */
  /*                    1/8 of full range (4095 <=> Vdda=3.3V): 512 <=> 0.41V */

  /* Analog watchdog 1 configuration */
  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_ALL_REG;
  AnalogWDGConfig.Channel = ADCx_CHANNELa;
  AnalogWDGConfig.ITMode = ENABLE;
  AnalogWDGConfig.HighThreshold = (RANGE_12BITS * 5/8);
  AnalogWDGConfig.LowThreshold = (RANGE_12BITS * 1/8);
  if (HAL_ADC_AnalogWDGConfig(&AdcHandle, &AnalogWDGConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    printk("ADC Analog WDG config failed \n");
  }

}

u16_t	Get_Adc(u32_t ch)
{
	ADC_ChannelConfTypeDef ADC1_ChanConf;
	ADC1_ChanConf.Channel=ch;
	ADC1_ChanConf.Rank=1;
	ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_41CYCLES_5;

	HAL_ADC_ConfigChannel(&AdcHandle,&ADC1_ChanConf);
	HAL_ADC_Start(&AdcHandle);
	HAL_ADC_PollForConversion(&AdcHandle,10);
	return (u16_t)HAL_ADC_GetValue(&AdcHandle);
}
