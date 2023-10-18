/**
 * MCU Support Package
*/
#include "py32f0xx_msp.h"

void MSP_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA | LL_IOP_GRP1_PERIPH_GPIOB | LL_IOP_GRP1_PERIPH_GPIOF);
  // PB0 RESET
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
  // PB1 DC/AO
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
  // PB2 CSN(XL2400)
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_2, LL_GPIO_MODE_OUTPUT);
  // PB3 CSN(ST7567)
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_OUTPUT);
  // PF4 Analog
  LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);
  // PB6 SH/LD
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);

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
