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

#ifdef USE_XL2400

/***************** XL2400 *****************/

// NSS:PB2
#define XL2400_NSS_LOW          LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2)
#define XL2400_NSS_HIGH         LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_2)
// Delay
#define XL2400_DELAY(__MS__)    LL_mDelay(__MS__)
// SPI TxRx
#define XL2400_SPI_TxRxByte(__DATA__)   SPI_TxRxByte(__DATA__)

#endif

#ifdef USE_XN297L

/***************** XN297L *****************/

// NSS:PB2
#define XN297L_CSN_LOW          LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2)
#define XN297L_CSN_HIGH         LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_2)
// SPI TxRx
#define XN297L_SPI_TxRxByte(__DATA__)   SPI_TxRxByte(__DATA__)
// NOP
#define XN297L_NOP               __NOP()

#endif

/***************** 74HC595 *****************/

// PB6: Storage register clock, RCLK or STCP
#define HC595_STCP_LOW          LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6)
#define HC595_STCP_HIGH         LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6)
// PA5: shift register clock, SRCLK or SHCP
#define HC595_SRCLK_LOW         LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5)
#define HC595_SRCLK_HIGH        LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5) 
// PA4: serial input, SER or DS
#define HC595_DS_LOW            LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4)
#define HC595_DS_HIGH           LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4)
// NOP
#define HC595_NOP               __NOP()

/***************** HSPWM          *****************/

/* PWM frequency, the count of PWM interval periods per second */
#define HSPWM_FREQUENCY 50
/* The resolution of each duty cycle */
#define HSPWM_PERIOD    120
/* PWM channels */
#define HSPWM_CH_SIZE   8

/***************** LSPWM(74HC595) *****************/

/* PWM frequency, the count of PWM interval periods per second */
#define LSPWM_FREQUENCY 50
/* The resolution of each duty cycle */
#define LSPWM_PERIOD    30
/* The number of 74hc595 */
#define HC595_SIZE      2
/* PWM channels, a multiple of 8 */
#define LSPWM_CH_SIZE   (HC595_SIZE*8)




void MSP_GPIO_Init(void);
void MSP_SPI_Init(void);
void MSP_TIM1_Config(void);
void MSP_TIM1_PWMConfig(void);
void MSP_TIM3_Config(void);
void MSP_TIM3_PWMConfig(void);
void MSP_TIM14_Config(void);

void MSP_FlashSetOptionBytes(void);
uint8_t SPI_TxRxByte(uint8_t data);


#ifdef __cplusplus
}
#endif

#endif /* __PY32F0XX_MSP_H */