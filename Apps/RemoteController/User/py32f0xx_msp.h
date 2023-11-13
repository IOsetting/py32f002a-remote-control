/******************************************************************************
 * 
 * \file        py32f0xx_msp.h
 * \author      IOsetting | iosetting@outlook.com
 * \date        
 * \brief       MCU Support Package
 * \note        
 *              
******************************************************************************/
#ifndef __PY32F0XX_MSP_H
#define __PY32F0XX_MSP_H


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/***************** ST7567 *****************/

// CSN: PB3
#define ST7567_CS_LOW       LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3)
#define ST7567_CS_HIGH      LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3)
// SCK: PF0 (SPI hardware)
// MOSI: PF2 (SPI hardware)
// Reset: PB0
#define ST7567_RESET_LOW    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0)
#define ST7567_RESET_HIGH   LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0)
// DC: PB1
#define ST7567_DC_LOW       LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1)
#define ST7567_DC_HIGH      LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1)
// Black Light
#define ST7567_BL_LOW       __NOP()
#define ST7567_BL_HIGH      __NOP()
// Delay
#define ST7567_DELAY(__MS__) LL_mDelay(__MS__)
// SPI TxRx
#define ST7567_SPI_TxRxByte(__DATA__)   SPI_TxRxByte(__DATA__)

/***************** XL2400 *****************/

// NSS:PB2
#define XL2400_NSS_LOW          LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2)
#define XL2400_NSS_HIGH         LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_2)
// Delay
#define XL2400_DELAY(__MS__)    LL_mDelay(__MS__)
// SPI TxRx
#define XL2400_SPI_TxRxByte(__DATA__)   SPI_TxRxByte(__DATA__)

/***************** 74HC165 *****************/

// PB6
#define HC165_LD_LOW            LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6)
#define HC165_LD_HIGH           LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6)
// PA6
#define HC165_SCK_LOW           LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_6)
#define HC165_SCK_HIGH          LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_6)
// PA7
#define HC165_DATA_READ         LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_7)
// NOP
#define HC165_NOP               __NOP()



void MSP_GPIO_Init(void);
void MSP_SPI_Init(void);

void MSP_DMA_Config(void);
void MSP_ADC_Init(void);
void MSP_TIM1_Init(void);

void MSP_FlashSetOptionBytes(void);
uint8_t SPI_TxRxByte(uint8_t data);


#ifdef __cplusplus
}
#endif

#endif /* __PY32F0XX_MSP_H */