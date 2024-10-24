/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "stm32g4xx_hal.h"
#include "gpio.h"
#include "dma.h"
#include "tim.h"
#include "crc.h"
#include "adc.h"

#include "main_state.h"

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
* Static Variables
*******************************************************************************/

/*******************************************************************************
* Static Function Prototypes
*******************************************************************************/

/**
  * \brief System Clock Configuration
  */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Configure the main internal regulator output voltage  */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

    /* Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure. */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN = 20;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}

/*!
 * \brief Initializes all timer in use
 */
static void MX_TIM_Init(void)
{
    /* TIM1: Main State Timer to obtain strictly cyclic calls */
    MX_TIM1_Init();

    /* TIM2: Measurement Timer Source to obtain 32-bit width micro-seconds timer */
    MX_TIM2_Init();

    /* TIM3: Start Siren Timer */
    MX_TIM3_Init();

    /* TIM4: Start Dimming Timer */
    MX_TIM4_Init();
    MX_TIM8_Init();
}

static void MX_ADC_Init(void)
{
    /* Init ADC 1 and 2 for Poti reading */
    MX_ADC1_Init();
    MX_ADC2_Init();

    /* Start ADC1 and ADC2 */
    HAL_ADC_Start(&hadc1);
    HAL_ADC_Start(&hadc2);
}

/*******************************************************************************
* Public Functions
*******************************************************************************/

/**
  * @brief  Jump entry from __main of compiler
  * @retval int
  */
int main(void)
{
    static MainState m_mainState;

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Stops the TIM1, TIM2 counting while debug breakpoint is hit */
    DBGMCU->APB1FZR1 |= DBGMCU_APB1FZR1_DBG_TIM2_STOP;
    DBGMCU->APB2FZ |= DBGMCU_APB2FZ_DBG_TIM1_STOP;

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_CRC_Init();
    MX_DMA_Init();
    MX_TIM_Init();
    MX_ADC_Init();

    MainState_Init(&m_mainState);

    while (1u)
    {
        MainState_Cyclic(&m_mainState);
    }

    return 0;
}



/**
  * \brief  This function is executed in case of error occurrence.
  */
void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
    uint16_t gpioPin = (uint16_t)__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_All);

    if (gpioPin)
    {
        HAL_GPIO_EXTI_IRQHandler(gpioPin);
    }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
