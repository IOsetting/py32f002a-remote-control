/******************************************************************************
** 
 * \file        st7567.c
 * \author      IOsetting | iosetting@outlook.com
 * \date        
 * \brief       Library of ST7567 LCD
 * \note        
 * \version     v0.1
 * \ingroup     demo
 *
 * 
******************************************************************************/

#include <string.h>
#include "st7567.h"

/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))

#if ST7567_X_ORIENT == ST7567_SEG_DIRECTION_REVERSE
    #define ST7567_X_OFFSET  ST7567_SEG_EXPAND
#else
    #define ST7567_X_OFFSET  0
#endif

/**
 * In datasheet, it says "the column address is increased (+1) after each display 
 * data access (read/write). This allows MPU accessing DDRAM content continuously. 
 * This feature stops at the end of each page (Column Address “83h”) because the 
 * Column Address and Page Address circuits are independent. For example, both Page 
 * Address and Column Address should be assigned for changing the DDRAM pointer 
 * from (Page-0, Column-83h) to (Page-1, Column-0)."
 * In actual test the Page Address will grow automatically.
*/

/* Private variable */
static uint8_t ST7567_Inverted = 0;


static void ST7567_TransmitByte(uint8_t dat)
{
    ST7567_CS_LOW;
    ST7567_SPI_TxRxByte(dat);
    ST7567_CS_HIGH;
}

static void ST7567_Transmit(const uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
    while (Size--)
    {
        ST7567_TransmitByte(*(pData++));
    }
}

void ST7567_WriteCommand(uint8_t command)
{
    ST7567_DC_LOW;
    ST7567_TransmitByte(command);
    ST7567_DC_HIGH;
}

void ST7567_WriteData(uint8_t data)
{
    ST7567_TransmitByte(data);
}

void ST7567_Init(void)
{
    ST7567_Reset();
    ST7567_BackLight_On();

    ST7567_WriteCommand(ST7567_RESET);
    ST7567_WriteCommand(ST7567_POWER_CONTROL
        |ST7567_POWER_CONTROL_VB
        |ST7567_POWER_CONTROL_VR
        |ST7567_POWER_CONTROL_VF); 
    ST7567_WriteCommand(ST7567_SET_EV);
    ST7567_WriteCommand(ST7567_SET_EV_MASK & 0x20);
    ST7567_WriteCommand(ST7567_BIAS_1_9);
    ST7567_WriteCommand(ST7567_X_ORIENT);
    ST7567_WriteCommand(ST7567_Y_ORIENT);
    ST7567_WriteCommand(ST7567_REGULATION_RATIO | ST7567_REGULATION_RATIO_5_0);
    ST7567_WriteCommand(ST7567_INVERSE_DISPLAY_OFF);
    ST7567_WriteCommand(ST7567_DISPLAY_ON);
    ST7567_WriteCommand(ST7567_ALL_PIXEL_NORMAL);

    ST7567_WriteCommand(ST7567_SET_START_LINE | (0x00 & ST7567_SET_START_LINE_MASK));
    ST7567_WriteCommand(ST7567_SET_PAGE_ADDRESS | (0x00 & ST7567_SET_PAGE_ADDRESS_MASK));
    ST7567_WriteCommand(ST7567_SET_COLUMN_ADDRESS_MSB);
    ST7567_WriteCommand(ST7567_SET_COLUMN_ADDRESS_LSB);
}

void ST7567_Reset(void)
{
    ST7567_RESET_LOW;
    ST7567_DELAY(0);
    ST7567_RESET_HIGH;
}

void ST7567_BackLight_On(void)
{
    ST7567_BL_HIGH;
}

void ST7567_BackLight_Off(void)
{
    ST7567_BL_LOW;
}

void ST7567_SetContrast(uint8_t val)
{
    ST7567_WriteCommand(ST7567_SET_EV);
    ST7567_WriteCommand(ST7567_SET_EV_MASK & val);
}

void ST7567_ToggleInvert(void) 
{
    /* Toggle invert */
    ST7567_Inverted = !ST7567_Inverted;
    if (ST7567_Inverted)
    {
        ST7567_WriteCommand(ST7567_INVERSE_DISPLAY_ON);
    }
    else
    {
        ST7567_WriteCommand(ST7567_INVERSE_DISPLAY_OFF);
    }
}

void ST7567_WritePage(uint8_t page, uint8_t column, const uint8_t *pData, uint32_t size) 
{
    ST7567_WriteCommand(ST7567_SET_PAGE_ADDRESS | (page & ST7567_SET_PAGE_ADDRESS_MASK));
    ST7567_WriteCommand(ST7567_SET_COLUMN_ADDRESS_MSB | ((column + ST7567_X_OFFSET) >> 4));
    ST7567_WriteCommand(ST7567_SET_COLUMN_ADDRESS_LSB | ((column + ST7567_X_OFFSET) & 0x0F));
    ST7567_Transmit(pData, size, ST7567_TIMEOUT);
}

void ST7567_FillPage(uint8_t page, uint8_t column, const uint8_t data, uint32_t size) 
{
    ST7567_WriteCommand(ST7567_SET_PAGE_ADDRESS | (page & ST7567_SET_PAGE_ADDRESS_MASK));
    ST7567_WriteCommand(ST7567_SET_COLUMN_ADDRESS_MSB | ((column + ST7567_X_OFFSET) >> 4));
    ST7567_WriteCommand(ST7567_SET_COLUMN_ADDRESS_LSB | ((column + ST7567_X_OFFSET) & 0x0F));
    while (size--)
    {
        ST7567_TransmitByte(data);
    }
}

void ST7567_FillAll(uint8_t data)
{
    uint8_t i = 0;
    for (i = 0; i < ST7567_PAGES; i++)
    {
        ST7567_FillPage(i, 0, data, ST7567_WIDTH);
    }
}
