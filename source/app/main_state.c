/**
 * @file      main_state.c
 * @author    Christian Hildenbrand
 * @date      01.05.2023
 *
 * @brief [description]
 */

/*******************************************************************************
* Includes
*******************************************************************************/

#include <stdbool.h>

#include "main_state.h"
#include "driver_blinky.h"
#include "driver_crc.h"
#include "stm32g4xx_hal_gpio.h"
#include "ctor_all.h"
#include "main.h"
#include "tim.h"
#include "adc.h"
#include "stm32g4xx_hal_tim.h"

/*******************************************************************************
* Defines
*******************************************************************************/

/*******************************************************************************
* Local Types and Typedefs
*******************************************************************************/

/*******************************************************************************
* Global Variables
*******************************************************************************/

/*******************************************************************************
* Static Function Prototypes
*******************************************************************************/

/*******************************************************************************
* Static Variables
*******************************************************************************/

/*******************************************************************************
* Functions
*******************************************************************************/

void MainState_Construct(MainState* const pThis, MainStateConfig const* const pCfg)
{
    assert_param(pThis != NULL);
    assert_param(pThis->constructed == false);
    assert_param(pCfg != NULL);

    pThis->pCfg = pCfg;

    pThis->initialized = false;
    pThis->constructed = true;
}

void MainState_Init(MainState* const pThis)
{
    assert_param(pThis != NULL);
    assert_param(pThis->initialized == false);
    assert_param(pThis->constructed == true);

    /* Construct all modules */
    CtorAll_Construct(pThis);

    RunState_Init(pThis->pCfg->pRunState);
    WaitState_Init(pThis->pCfg->pWaitState);

    HAL_GPIO_WritePin(LD2_OUTPUT_GPIO_Port, LD2_OUTPUT_Pin, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(EDU_LD0_GPIO_Port, EDU_LD0_Pin, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(EDU_LD1_GPIO_Port, EDU_LD1_Pin, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(EDU_LD2_GPIO_Port, EDU_LD2_Pin, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(EDU_LD3_GPIO_Port, EDU_LD3_Pin, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(EDU_LD4_GPIO_Port, EDU_LD4_Pin, GPIO_PIN_SET);

    HAL_TIM_PWM_Start(&htim)



    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);

    pThis->data.state = MainState_Run;

    pThis->initialized = true;
}

void MainState_Cyclic(MainState* const pThis)
{
    assert_param(pThis != NULL);
    assert_param(pThis->constructed == true);
    assert_param(pThis->initialized == true);

    pThis->data.cycleCounter++;

    /*
     * 40%
     * 20%
     * 0%
     * 20%
     * 40%
     *
     * LD0 -> TIM8_CH3N
     * LD1 -> TIM2_CH3
     * LD2 -> TIM3_CH2
     * LD3 -> TIM4_CH4
     * LD4 -> TIM4_CH3
     *
     */

    uint16_t adc1_val = HAL_ADC_GetValue(&hadc1);
    //uint16_t adc2_val = HAL_ADC_GetValue(&hadc2);

    uint64_t ledAutoReloadVal = 1000;
    uint64_t ledDutyCycle = (uint64_t)(adc1_val / 41.0f);

    uint64_t buzzerAutoReloadVal = 1000 + (pThis->data.cycleCounter % 2000);
    uint64_t buzzerDutyCycle = (uint64_t)(adc1_val / 41.0f);

    uint64_t buzzerDutyCycleInTicks = (uint64_t)((buzzerAutoReloadVal / 100.0f) * buzzerDutyCycle);
    uint64_t ledDutyCycleInTicks = (uint64_t)((ledAutoReloadVal / 100.0f) * ledDutyCycle);

    LL_TIM_SetAutoReload(htim3.Instance, buzzerAutoReloadVal);
    LL_TIM_OC_SetCompareCH2(htim3.Instance, buzzerDutyCycleInTicks);

    LL_TIM_OC_SetCompareCH3(htim4.Instance, ledDutyCycleInTicks);

    if (pThis->data.cycleCounter % 500 == 0)
    {
        HAL_GPIO_TogglePin(LD2_OUTPUT_GPIO_Port, LD2_OUTPUT_Pin);
    }

    if (pThis->data.state == MainState_Run)
    {
        RunState_Cyclic(pThis->pCfg->pRunState);

        pThis->data.state = MainState_Wait;
    }

    else if (pThis->data.state == MainState_Wait)
    {
        WaitState_Cyclic(pThis->pCfg->pWaitState);

        if (WaitState_IsError(pThis->pCfg->pWaitState))
        {
            pThis->data.state = MainState_Error;
        }

        else
        {
            pThis->data.state = MainState_Run;
        }
    }

    else
    {
        Error_Handler();
    }
}


