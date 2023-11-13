#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG 1


#if DEBUG == 0
    #define DEBUG_PRINTF(__ARG1__, __ARG2__)    (void)(__ARG1__);(void)(__ARG2__)
    #define DEBUG_PRINT_STRING(__ARG1__)        (void)(__ARG1__)
#else

#include "SEGGER_RTT.h"

    #define DEBUG_PRINTF(__ARG1__, __ARG2__)    SEGGER_RTT_printf(0, __ARG1__, __ARG2__);
    #define DEBUG_PRINT_STRING(__ARG1__)        SEGGER_RTT_WriteString(0, __ARG1__);
#endif

/* PWM frequency, the count of PWM interval periods per second */
#define PWM_FREQUENCY 50
/* The resolution of each duty cycle */
#define PWM_PERIOD    30
/* The count of 74hc595 */
#define HC595_SIZE    2
/* PWM channels, a multiple of 8 */
#define PWM_CH_SIZE   (HC595_SIZE*8)


#include "py32f0xx_ll_adc.h"
#include "py32f0xx_ll_bus.h"
#include "py32f0xx_ll_cortex.h"
#include "py32f0xx_ll_dma.h"
#include "py32f0xx_ll_exti.h"
#include "py32f0xx_ll_gpio.h"
#include "py32f0xx_ll_flash.h"
#include "py32f0xx_ll_pwr.h"
#include "py32f0xx_ll_rcc.h"
#include "py32f0xx_ll_spi.h"
#include "py32f0xx_ll_system.h"
#include "py32f0xx_ll_tim.h"
#include "py32f0xx_ll_utils.h"


void APP_ErrorHandler(void);
void APP_TIM1UpdateCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
