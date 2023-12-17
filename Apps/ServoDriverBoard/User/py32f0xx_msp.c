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

void MSP_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA | LL_IOP_GRP1_PERIPH_GPIOB | LL_IOP_GRP1_PERIPH_GPIOF);

  // PB2 CSN(XL2400)
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_2, LL_GPIO_MODE_OUTPUT);
  // PF4 Analog(Inhibit PF4 for PB6)
  LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);

  /* PB6 RCLK/STCP */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /* PA5 SRCLK/SHCP */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* PA4 SER/DS */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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

void MSP_TIM1_Config(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP2_EnableClock(RCC_APBENR2_TIM1EN);

  TIM_InitStruct.ClockDivision       = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.CounterMode         = LL_TIM_COUNTERMODE_UP;
  /*
   * MCU clock = 48,000,000, Prescaler = 8000, Autoreload = 120
   * PWM freq = 50 = 48,000,000 / 8000 / 120
   * PWM period = Autoreload = 120
  */
  TIM_InitStruct.Prescaler           = (SystemCoreClock / (HSPWM_FREQUENCY * HSPWM_PERIOD)) -1;
  TIM_InitStruct.Autoreload          = HSPWM_PERIOD - 1;
  TIM_InitStruct.RepetitionCounter   = 0;
  LL_TIM_Init(TIM1,&TIM_InitStruct);
  LL_TIM_EnableAllOutputs(TIM1);
  LL_TIM_EnableCounter(TIM1);
}

void MSP_TIM1_PWMConfig(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_Initstruct = {0};

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /* PA0/PA1/PA3 -> AF13 -> TIM1_CH3/TIM1_CH4/TIM1_CH1 */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_13;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* PB3 -> AF1 -> TIM1_CH2 */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  TIM_OC_Initstruct.OCMode = LL_TIM_OCMODE_PWM2;
  TIM_OC_Initstruct.OCState = LL_TIM_OCSTATE_ENABLE;
  TIM_OC_Initstruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_Initstruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;

  /* Set channel compare values */
  TIM_OC_Initstruct.CompareValue = 0;
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH1, &TIM_OC_Initstruct);
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH2, &TIM_OC_Initstruct);
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH3, &TIM_OC_Initstruct);
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH4, &TIM_OC_Initstruct);
}

void MSP_TIM3_Config(void)
{
  LL_TIM_InitTypeDef TIM3CountInit = {0};

  LL_APB1_GRP1_EnableClock(RCC_APBENR1_TIM3EN);

  TIM3CountInit.ClockDivision       = LL_TIM_CLOCKDIVISION_DIV1;
  TIM3CountInit.CounterMode         = LL_TIM_COUNTERMODE_UP;
  /*
   * MCU clock = 48,000,000, Prescaler = 8000, Autoreload = 120
   * PWM freq = 50 = 48,000,000 / 8000 / 120
   * PWM period = Autoreload = 120
  */
  TIM3CountInit.Prescaler           = (SystemCoreClock / (HSPWM_FREQUENCY * HSPWM_PERIOD)) -1;
  TIM3CountInit.Autoreload          = HSPWM_PERIOD - 1;
  TIM3CountInit.RepetitionCounter   = 0;
  LL_TIM_Init(TIM3, &TIM3CountInit);
  LL_TIM_EnableAllOutputs(TIM3);
  LL_TIM_EnableCounter(TIM3);
}

void MSP_TIM3_PWMConfig(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_Initstruct = {0};

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /* PA6/PA7 -> AF1 -> TIM3_CH1/TIM3_CH2 */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* PB0/PB1 -> AF1 -> TIM3_CH3/TIM3_CH4 */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  TIM_OC_Initstruct.OCMode = LL_TIM_OCMODE_PWM2;
  TIM_OC_Initstruct.OCState = LL_TIM_OCSTATE_ENABLE;
  TIM_OC_Initstruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_Initstruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;

  /* Set channel compare values */
  TIM_OC_Initstruct.CompareValue = 0;
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH1, &TIM_OC_Initstruct);
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH2, &TIM_OC_Initstruct);
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH3, &TIM_OC_Initstruct);
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH4, &TIM_OC_Initstruct);
}

void MSP_TIM14_Config(void)
{
  LL_TIM_InitTypeDef TIM14Init = {0};

  LL_APB1_GRP2_EnableClock(RCC_APBENR2_TIM14EN);

  TIM14Init.ClockDivision       = LL_TIM_CLOCKDIVISION_DIV1;
  TIM14Init.CounterMode         = LL_TIM_COUNTERMODE_UP;
  /*
   * MCU clock = 48,000,000, Prescaler = 8000, Autoreload = 4
   * TIMER freq = 1500 = 48,000,000 / 8000 / 4
   * LSPWM freq = 50 = 1500 / 30
   * LSPWM period = 30
  */
  TIM14Init.Prescaler           = 8000-1;
  TIM14Init.Autoreload          = (SystemCoreClock / 8000) / (LSPWM_FREQUENCY * LSPWM_PERIOD) -1;
  TIM14Init.RepetitionCounter   = 0;
  LL_TIM_Init(TIM14, &TIM14Init);

  LL_TIM_EnableIT_UPDATE(TIM14);
  LL_TIM_EnableCounter(TIM14);

  NVIC_EnableIRQ(TIM14_IRQn);
  NVIC_SetPriority(TIM14_IRQn, 1);
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
