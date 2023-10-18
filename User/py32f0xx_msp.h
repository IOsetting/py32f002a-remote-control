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

void MSP_GPIO_Init(void);
void MSP_SPI_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __PY32F0XX_MSP_H */