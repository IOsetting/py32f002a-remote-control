/**
 * MCU Support Package
 * 
 * PY32F002A Pinout
 * 
 *               ___________
 *          GND -| 1    20 |- VCC
 *         PA13 -| 2    19 |- PB1
 *         PA14 -| 3    18 |- PB2
 *          PB3 -| 4    17 |- PB0
 * PB6/PF4/BOOT0-| 5    16 |- PA7
 *          PA6 -| 6    15 |- PA4
 *          PA5 -| 7    14 |- PA3
 *          PF0 -| 8    13 |- PA2
 *          PF1 -| 9    12 |- PA1
 *      PF2/RST -| 10   11 |- PA0
 *               -----------
 * 
 * 
 * |         |       |           | ST7567 | XL2400 | joystick_01 | joystick_02 | ptmeter_01 | ptmeter_02 | 74HC165   |    |  
 * | ---     | ---   | ---       | ---    | ---    | ---         | ---         | ---        | ---        | ----      | -- | 
 * | PA0     |       |           |        |        |   X         |             |            |            |           |    |   
 * | PA1     |       |           |        |        |   x         |             |            |            |           |    |   
 * | PA2     |       |           |        |        |             |  X          |            |            |           |    |   
 * | PA3     |       |           |        |        |             |  X          |            |            |           |    | 
 * | PA4     |       |           |        |        |             |             | X          |            |           |    |
 * | PA5     |       |           |        |        |             |             |            | X          |           |    |
 * | PA6     |       |           |        |        |             |             |            |            | CLK       |    |
 * | PA7     |       |           |        |        |             |             |            |            | QH        |    |
 * | PA13    | SWD   |           |        |        |             |             |            |            |           |    |
 * | PA14    | SWC   |           |        |        |             |             |            |            |           |    |
 * | PB0     |       |           | RESET  |        |             |             |            |            |           |    |
 * | PB1     |       |           | DC/AO  |        |             |             |            |            |           |    |
 * | PB2     |       |           |        |   CSN  |             |             |            |            |           |    |
 * | PB3     |       |           | CSN/CE |        |             |             |            |            |           |    |
 * | PF0     |       | SPI2_SCK  | SCK    |   SCK  |             |             |            |            |           |    |
 * | PF1     |       | SPI2_MISO |        |   DATA |             |             |            |            |           |    |
 * | PF2     | NRST  | SPI2_MOSI | MOSI   |   DATA |             |             |            |            |           |    |
 * | PF4/PB6 | BOOT0 |           |        |        |             |             |            |            | SH/LD     |    |
 * 
 * 
 * |         |       |           | XL2400 |     PWM     | 74HC595    |        |  
 * | ---     | ---   | ---       | ------ | -------     | ---------  | ------ | 
 * | PA0     |       |           |        |   TIM1_3    |            |        |   
 * | PA1     |       |           |        |   TIM1_4    |            |        |   
 * | PA2     |       |           |        |   TIM3_1    |            |        |   
 * | PA3     |       |           |        |   TIM1_1    |            |        | 
 * | PA4     |       |           |        |             | SER/DS     |        |
 * | PA5     |       |           |        |             | SRCLK/SHCP |        |
 * | PA6     |       |           |        |   TIM3_1    |            |        |
 * | PA7     |       |           |        |   TIM3_2    |            |        |
 * | PA13    | SWD   |           |        |             |            |        |
 * | PA14    | SWC   |           |        |             |            |        |
 * | PB0     |       |           |        |   TIM3_3    |            |        |
 * | PB1     |       |           |        |   TIM3_4    |            |        |
 * | PB2     |       |           |   CSN  |             |            |        |
 * | PB3     |       |           |        |   TIM1_2    |            |        |
 * | PF0     |       | SPI2_SCK  |   SCK  |             |            |        |
 * | PF1     |       | SPI2_MISO |   DATA |             |            |        |
 * | PF2     | NRST  | SPI2_MOSI |   DATA |             |            |        |
 * | PF4/PB6 | BOOT0 |           |        |             | RCLK/STCP  |        |
 * 
 * 
*/
#include "py32f0xx_msp.h"

extern __IO uint16_t adc_dma_data[6];

void MSP_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA | LL_IOP_GRP1_PERIPH_GPIOB | LL_IOP_GRP1_PERIPH_GPIOF);
  // PB0 RESET(ST7567)
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
  // PB1 DC/AO(ST7567)
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
  // PB2 CSN(XL2400)
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_2, LL_GPIO_MODE_OUTPUT);
  // PB3 CSN(ST7567)
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_OUTPUT);
  // PF4 Analog(Inhibit PF4 for PB6)
  LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);
  // PA6 CLK(74HC165)
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
  // PB6 SH/LD(74HC165)
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
  // PA7 QH(74HC165)
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_INPUT);

  // PF0 AF3 = SPI2 SCK
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_3;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  // PF1 AF3 = SPI2 MISO
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_3;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  // PF2 AF3 = SPI2 MOSI
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_3;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

void MSP_SPI_Init(void)
{
  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV16;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  LL_SPI_Init(SPI2, &SPI_InitStruct);
  LL_SPI_Enable(SPI2);
}

void MSP_DMA_Config(void)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);

  // Remap ADC to LL_DMA_CHANNEL_1
  LL_SYSCFG_SetDMARemap_CH1(LL_SYSCFG_DMA_MAP_ADC);
  // Transfer from peripheral to memory
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  // Set priority
  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_HIGH);
  // Circular mode
  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);
  // Peripheral address no increment
  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);
  // Memory address increment
  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);
  // Peripheral data alignment : 16bit
  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_HALFWORD);
  // Memory data alignment : 16bit
  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_HALFWORD);
  // Data length
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, 6);
  // Sorce and target address
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&ADC1->DR, (uint32_t)adc_dma_data, LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1));
  // Enable DMA channel 1
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
  // Enable transfer-complete interrupt
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);

  NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void MSP_ADC_Init(void)
{
  __IO uint32_t backup_setting_adc_dma_transfer = 0;

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_ADC1);

  LL_ADC_Reset(ADC1);
  // Calibrate start
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Backup current settings */
    backup_setting_adc_dma_transfer = LL_ADC_REG_GetDMATransfer(ADC1);
    /* Turn off DMA when calibrating */
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);
    LL_ADC_StartCalibration(ADC1);

    while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0);

    /* Delay 1ms(>= 4 ADC clocks) before re-enable ADC */
    LL_mDelay(1);
    /* Apply saved settings */
    LL_ADC_REG_SetDMATransfer(ADC1, backup_setting_adc_dma_transfer);
  }
  // Calibrate end

  /* PA0 ~ PA5 as ADC input */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_4| LL_GPIO_PIN_5, LL_GPIO_MODE_ANALOG);
  /* Set ADC channel and clock source when ADEN=0, set other configurations when ADSTART=0 */
  LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_NONE);

  LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV2);
  LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
  LL_ADC_SetResolution(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
  LL_ADC_SetLowPowerMode(ADC1, LL_ADC_LP_MODE_NONE);
  LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_41CYCLES_5);

  /* Set TIM1 as trigger source */
  LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_EXT_TIM1_TRGO);
  LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);
  /* Single conversion mode (CONT = 0, DISCEN = 0), performs a single sequence of conversions, converting all the channels once */
  LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);

  LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
  LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);
  /* Enable: each conversions in the sequence need to be triggerred separately */
  LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_DISABLE);
  /* Set channel 0/1/2/3/4/5 */
  LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_0 | LL_ADC_CHANNEL_1 | LL_ADC_CHANNEL_2 | LL_ADC_CHANNEL_3 | LL_ADC_CHANNEL_4 | LL_ADC_CHANNEL_5);

  LL_ADC_Enable(ADC1);

  // Start ADC regular conversion
  LL_ADC_REG_StartConversion(ADC1);
}

void MSP_TIM1_Init(void)
{
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM1);
  LL_TIM_SetPrescaler(TIM1, (SystemCoreClock / 6000) - 1);
  LL_TIM_SetAutoReload(TIM1, 6000 - 1);
  /* Triggered by update */
  LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_UPDATE);

  LL_TIM_EnableCounter(TIM1);
}

void MSP_FlashSetOptionBytes(void)
{
  FLASH_OBProgramInitTypeDef OBInitCfg;

  LL_FLASH_Unlock();
  LL_FLASH_OB_Unlock();

  OBInitCfg.OptionType = OPTIONBYTE_USER;
  OBInitCfg.USERType = OB_USER_BOR_EN | OB_USER_BOR_LEV | OB_USER_IWDG_SW | OB_USER_WWDG_SW | OB_USER_NRST_MODE | OB_USER_nBOOT1;
  /*
   * The default value: OB_BOR_DISABLE | OB_BOR_LEVEL_3p1_3p2 | OB_IWDG_SW | OB_WWDG_SW | OB_RESET_MODE_RESET | OB_BOOT1_SYSTEM;
  */
  OBInitCfg.USERConfig = OB_BOR_DISABLE | OB_BOR_LEVEL_3p1_3p2 | OB_IWDG_SW | OB_WWDG_SW | OB_RESET_MODE_GPIO | OB_BOOT1_SYSTEM;
  LL_FLASH_OBProgram(&OBInitCfg);

  LL_FLASH_Lock();
  LL_FLASH_OB_Lock();
  /* Reload option bytes */
  LL_FLASH_OB_Launch();
}

uint8_t SPI_TxRxByte(uint8_t data)
{
  uint8_t SPITimeout = 0xFF;
  /* Check the status of Transmit buffer Empty flag */
  while (READ_BIT(SPI2->SR, SPI_SR_TXE) == RESET)
  {
    if (SPITimeout-- == 0) return 0;
  }
  LL_SPI_TransmitData8(SPI2, data);
  SPITimeout = 0xFF;
  while (READ_BIT(SPI2->SR, SPI_SR_RXNE) == RESET)
  {
    if (SPITimeout-- == 0) return 0;
  }
  // Read from RX buffer
  return LL_SPI_ReceiveData8(SPI2);
}
