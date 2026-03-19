/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define BUTTON_PRESS  0U
#define BUTTON_RELEASE 1U

#define XILANH_DOWN()  	HAL_GPIO_WritePin(Xilanh_GPIO_Port, Xilanh_Pin,GPIO_PIN_SET)
#define XILANH_UP()		HAL_GPIO_WritePin(Xilanh_GPIO_Port, Xilanh_Pin,GPIO_PIN_RESET)
#define XILANH_Test()	HAL_GPIO_TogglePin(Xilanh_GPIO_Port, Xilanh_Pin)



#ifndef TIME_OUT
#define TIME_OUT 850
#endif

typedef enum {
    IDLE,           // Chưa nhấn RL → không làm gì
    WAIT_SENSOR,    // Đang chờ sensor sau khi nhấn RL
    COUNTING        // Sensor đã kích → đang đếm timeout
} state_t;

volatile state_t system_state = IDLE;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

__ALIGNED(1) volatile uint8_t 	RL_button_stt  = BUTTON_RELEASE,
					Emergency_button_stt  = BUTTON_RELEASE,
					Sensor_stt = BUTTON_RELEASE;
__ALIGNED(1) volatile uint32_t G32_Timer_tick = 0;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int hoang(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Xilanh_GPIO_Port, Xilanh_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Xilanh_Pin */
  GPIO_InitStruct.Pin = Xilanh_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Xilanh_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RL_Button_Pin Sensor_Pin Emergency_Button_Pin */
  GPIO_InitStruct.Pin = RL_Button_Pin|Sensor_Pin|Emergency_Button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

#if 0
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	  if(GPIO_Pin == RL_Button_Pin){
		  if(!HAL_GPIO_ReadPin(RL_Button_GPIO_Port, RL_Button_Pin)){
//			  G32_Timer_tick = 0;
//				Sensor_stt =  BUTTON_RELEASE;
				XILANH_DOWN();
		  }
	  }
	  else if(GPIO_Pin == Emergency_Button_Pin){
		  if(!HAL_GPIO_ReadPin(Emergency_Button_GPIO_Port, Emergency_Button_Pin)){
			  XILANH_UP();
			  G32_Timer_tick = 0;
			Sensor_stt =  BUTTON_RELEASE;
		  }
	  }
	  else if(GPIO_Pin == Sensor_Pin){
		  if(!HAL_GPIO_ReadPin(Sensor_GPIO_Port, Sensor_Pin)){
			  G32_Timer_tick = 0;
			  Sensor_stt =  BUTTON_PRESS;
		  }
	  }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim1){
		if(Sensor_stt ==  BUTTON_PRESS){
			G32_Timer_tick ++ ;
			if(G32_Timer_tick >= TIME_OUT){
				G32_Timer_tick = 0;
				 XILANH_UP();
				 Sensor_stt =  BUTTON_RELEASE;
			}
		}
	}
}
#endif
#if 1
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == RL_Button_Pin) {
        if (!HAL_GPIO_ReadPin(RL_Button_GPIO_Port, RL_Button_Pin)) {

            if (system_state == IDLE) {
                XILANH_DOWN();
                G32_Timer_tick = 0;
                system_state = WAIT_SENSOR;
            }
            // nếu đang WAIT_SENSOR hoặc COUNTING → bỏ qua nhấn thứ 2
        }
    }

    else if (GPIO_Pin == Sensor_Pin) {
        if (!HAL_GPIO_ReadPin(Sensor_GPIO_Port, Sensor_Pin)) {

            if (system_state == WAIT_SENSOR) {
                // chỉ nhận sensor 1 lần sau khi nhấn RL
                G32_Timer_tick = 0;
                system_state = COUNTING;
            }
            // nếu COUNTING hoặc IDLE → bỏ qua
        }
    }

    else if (GPIO_Pin == Emergency_Button_Pin) {
        if (!HAL_GPIO_ReadPin(Emergency_Button_GPIO_Port, Emergency_Button_Pin)) {
            XILANH_UP();
            G32_Timer_tick = 0;
            system_state = IDLE;   // reset state
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim1) {

        if (system_state == COUNTING) {
            G32_Timer_tick++;

            if (G32_Timer_tick >= TIME_OUT) {
                G32_Timer_tick = 0;
                XILANH_UP();
                system_state = IDLE;
            }
        }
    }
}
#endif
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
