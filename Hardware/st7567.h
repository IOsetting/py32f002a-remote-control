/******************************************************************************
** 
 * \file        st7567.h
 * \author      IOsetting | iosetting@outlook.com
 * \date        
 * \brief       Library of ST7567 LCD
 * \note        
 * \version     v0.1
 * \ingroup     demo
 *              
******************************************************************************/

#ifndef __ST7567_H_
#define __ST7567_H_

#include "py32f0xx_msp.h"

/**
 * Required Pin Definitions:
 * 
 * #define ST7567_CS_LOW       LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3)
 * #define ST7567_CS_HIGH      LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3)
 * #define ST7567_RESET_LOW    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0)
 * #define ST7567_RESET_HIGH   LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0)
 * #define ST7567_DC_LOW       LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1)
 * #define ST7567_DC_HIGH      LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1)
 * #define ST7567_BL_LOW       __NOP()
 * #define ST7567_BL_HIGH      __NOP()
 * #define ST7567_DELAY(__MS__) LL_mDelay(__MS__)
 * #define ST7567_SPI_TxRxByte(__DATA__)   SPI_TxRxByte(__DATA__)
*/

// X width
#define ST7567_WIDTH  128
// Y height
#define ST7567_HEIGHT 64
// Additional bytes in each row
#define ST7567_SEG_EXPAND 4
// Display RAM Pages (8x8bit + 1bit)
#define ST7567_PAGES 8
// X orientation
#define ST7567_X_ORIENT ST7567_SEG_DIRECTION_NORMAL
// Y orientation
#define ST7567_Y_ORIENT ST7567_COM_DIRECTION_REVERSE


/* ST7567 commands definitions */
#define ST7567_DISPLAY_OFF                   0xAE /* 0xae: Display OFF (sleep mode) */
#define ST7567_DISPLAY_ON                    0xAF /* 0xaf: Display ON in normal mode */

#define ST7567_SET_START_LINE                0x40 /* 0x40-7f: Set display start line */
#define  ST7567_SET_START_LINE_MASK          0x3f

#define ST7567_SET_PAGE_ADDRESS              0xB0 /* 0xb0-b7: Set page start address */
#define  ST7567_SET_PAGE_ADDRESS_MASK        0x07

#define ST7567_SET_COLUMN_ADDRESS_MSB        0x10 /* 0x10-0x1f: Set higher column address */
#define ST7567_SET_COLUMN_ADDRESS_MSB_MASK   0x0f

#define ST7567_SET_COLUMN_ADDRESS_LSB        0x00 /* 0x00-0x0f: Set lower column address */
#define  ST7567_SET_COLUMN_ADDRESS_LSB_MASK  0x0F

/**
 * SEG: 0 - 131
*/
#define ST7567_SEG_DIRECTION_NORMAL          0xA0 /* 0xa0: Column address 0 is mapped to SEG0 */
#define ST7567_SEG_DIRECTION_REVERSE         0xA1 /* 0xa1: Column address 128 is mapped to SEG0 */

/**
 * COM: 0 - 63
*/
#define ST7567_COM_DIRECTION_NORMAL          0xC0 /* 0xc0: Set COM output direction, normal mode */
#define ST7567_COM_DIRECTION_REVERSE         0xC8 /* 0xc8: Set COM output direction, reverse mode */

#define ST7567_INVERSE_DISPLAY_OFF           0xA6 /* 0xa6: Normal display */
#define ST7567_INVERSE_DISPLAY_ON            0xA7 /* 0xa7: Inverse display */

#define ST7567_ALL_PIXEL_ON                  0xA5 /* 0xa5: Entire display ON */
#define ST7567_ALL_PIXEL_NORMAL              0xA4 /* 0xa4: Resume to RAM content display */

#define ST7567_BIAS_1_9                      0xA2 /* 0xa2: Select BIAS setting 1/9 */
#define ST7567_BIAS_1_7                      0xA3 /* 0xa3: Select BIAS setting 1/7 */

#define ST7567_READ_MODIFY_WRITE_START       0xE0 /* 0xe0: Enter the Read Modify Write mode */
#define ST7567_READ_MODIFY_WRITE_END         0xEE /* 0xee: Leave the Read Modify Write mode */
#define ST7567_RESET                         0xE2 /* 0xe2: Software RESET */

/**
 * This instruction controls the built-in power circuits. 
 * Typically, these 3 flags are turned ON at the same time.
*/
#define ST7567_POWER_CONTROL                 0x28
#define ST7567_POWER_CONTROL_VF              0x01
#define ST7567_POWER_CONTROL_VR              0x02
#define ST7567_POWER_CONTROL_VB              0x04

/**
 * The operation voltage (V0) calculation formula is shown below: 
 * (RR comes from Regulation Ratio, EV comes from EV[5:0])
 * V0 = RR X [ 1 – (63 – EV) / 162 ] X 2.1, or 
 * V0 = RR X [ ( 99 + EV ) / 162 ] X 2.1
*/
#define ST7567_REGULATION_RATIO              0x20
#define ST7567_REGULATION_RATIO_3_0          0x00
#define ST7567_REGULATION_RATIO_3_5          0x01
#define ST7567_REGULATION_RATIO_4_0          0x02
#define ST7567_REGULATION_RATIO_4_5          0x03
#define ST7567_REGULATION_RATIO_5_0          0x04 /* Default */
#define ST7567_REGULATION_RATIO_5_5          0x05
#define ST7567_REGULATION_RATIO_6_0          0x06
#define ST7567_REGULATION_RATIO_6_5          0x07

/**
 * This is double byte instruction. The first byte set ST7567 into EV 
 * adjust mode and the following instruction will change the EV setting. 
 * That means these 2 bytes must be used together. They control the electronic 
 * volume to adjust a suitable V0 voltage for the LCD.
*/
#define ST7567_SET_EV                        0x81
#define ST7567_SET_EV_MASK                   0x3F

#define ST7567_SET_BOOSTER                   0xF8 /* Set booster level */
#define ST7567_SET_BOOSTER_4X                0x00
#define ST7567_SET_BOOSTER_5X                0x01

#define ST7567_NOP                           0xE3
#define ST7567_TEST                          0xFE

#ifndef ST7567_TIMEOUT
#define ST7567_TIMEOUT					20000
#endif

/** Background color */
#define ST7567_COLOR_BACK 0x00
/** Front color */
#define ST7567_COLOR_FRONT 0x01

/**
 * @brief  Initializes ST7567 LCD
 * @param  None
 * @retval None
 */
void ST7567_Init(void);

/**
 * @brief  Hardware reset ST7567 LCD
 * @param  None
 * @retval None
 */
void ST7567_Reset(void);

/**
 * @brief  Turn ST7567 LCD backlight on
 * @param  None
 * @retval None
 */
void ST7567_BackLight_On(void);

/**
 * @brief  Turn ST7567 LCD backlight off
 * @param  None
 * @retval None
 */
void ST7567_BackLight_Off(void);

/**
 * @brief  Turn ST7567 LCD backlight off
 * @param  val: value between 0x00 ~ 0x3F
 * @retval None
 */
void ST7567_SetContrast(uint8_t val);

/**
 * @brief  Toggles pixels invertion inside internal RAM
 * @note   @ref ST7567_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  None
 * @retval None
 */
void ST7567_ToggleInvert(void);

void ST7567_SetCursor(uint8_t page, uint8_t column);

void ST7567_TransmitByte(uint8_t dat);

void ST7567_TransmitBytes(const uint8_t *pData, uint32_t size);

void ST7567_WriteByPage(uint8_t page, uint8_t column, const uint8_t *pData, uint32_t size);

void ST7567_FillByPage(uint8_t page, uint8_t column, const uint8_t data, uint32_t size);

/** 
 * @brief  Fills entire LCD with desired color
 * @note   @ref ST7567_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  Color: Color to be used for screen fill. This parameter can be a value of @ref ST7567_COLOR_t enumeration
 * @retval None
 */
void ST7567_FillAll(uint8_t Color);


#endif // __ST7567_H_
