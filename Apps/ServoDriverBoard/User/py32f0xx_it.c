#include "main.h"
#include "drv_lspwm.h"
#include "py32f0xx_it.h"

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
}

extern uint16_t device_idle_counter;

void TIM14_IRQHandler(void)
{
  if(LL_TIM_IsActiveFlag_UPDATE(TIM14) && LL_TIM_IsEnabledIT_UPDATE(TIM14))
  {
    LL_TIM_ClearFlag_UPDATE(TIM14);
    device_idle_counter++;
    DRV_LSPWM_Tick();
  }
}
