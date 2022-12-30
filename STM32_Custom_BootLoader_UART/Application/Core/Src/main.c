/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "crc.h"
#include "iwdg.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define BOOT_LOADER_ADDRESS 0x08000000U
#define VECTOR_TABLE_OFFSET 0x0U
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define bufferSize 255
uint8_t Rx_data[2] = {0}, Rx_Buffer[bufferSize] = {0};
uint8_t Transfer_cplt = 0, Rx_indx = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint8_t sample = 0;
  for(uint8_t i = 0; i <= 8 ; i++)
  {
    if(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == 0)
    {
      sample++;
      if(sample >= 5)
      HAL_GPIO_TogglePin(GLED_GPIO_Port,GLED_Pin);
    }
  }
  
}

void jumpToBootLoader(void)
{
  //__disable_irq();
  // pointer function to call bootLoader Reset Handler
  void(*resetHandler)(void);
  //reading MSP value of bootLoader Application in start Address of Boot loader App
  //boot loader start at 0x0800,0000U
  uint32_t mspValue = *(volatile uint32_t *)BOOT_LOADER_ADDRESS;
  // Seting the MSP
  __set_SP(mspValue);
  // Change the Vector Table Offcest to Application Vector Table
  SCB->VTOR = BOOT_LOADER_ADDRESS | VECTOR_TABLE_OFFSET;
  // jumping to the reset handeler of the user bootLoader
  // now fetch the reset handler address of the user bootLoader form the 
  // location BOOT_LOADER_ADDRESS + 4 
  resetHandler = (void(*)(void))(*(volatile uint32_t*)(BOOT_LOADER_ADDRESS + 0x00000004U));
  resetHandler();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART2)
  {
//    if (Rx_indx == 0) 
//    {
//      for (uint8_t i = 0; i < bufferSize; i++) 
//        Rx_Buffer[i] = 0; 
//    }   
    if(Rx_data[0] != '\n') //if received data different from ascii 13 (enter)
    {
      Rx_Buffer[Rx_indx++] = Rx_data[0];     //add data to Rx_Buffer
    }
    else            //if received data = 13
    {
      Rx_indx = 0;
      Transfer_cplt = 1; //received complete, data is ready to read
    }
    HAL_UART_Receive_IT(&huart2, Rx_data, 1);
  }
  else
  {}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
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
  MX_CRC_Init();
  MX_USART2_UART_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GLED_GPIO_Port,GLED_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(RLED_GPIO_Port,RLED_Pin,GPIO_PIN_SET);
  /* USER CODE END 2 */
<<<<<<< HEAD
  HAL_IWDG_Refresh(&hiwdg);
  HAL_UART_Receive_IT(&huart2, Rx_data, 1);
  HAL_UART_Transmit(&huart2,"Application Start\r\n",19,500);
=======
  HAL_FLASH_Unlock();
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,0x8007F90,8585);
   HAL_FLASH_Lock();
>>>>>>> 3831ada184fb707c389b21c64c547f811064e1c7
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    if(Transfer_cplt == 1)
    {
      Transfer_cplt = 0;
      if(Rx_Buffer[0] == 0x40)
      {
        HAL_UART_Transmit(&huart2,"Jump To BootLoader\r\n",20,500);
       *(volatile uint8_t*)0x20004FFF = 0x20U;
       *(volatile uint8_t*)0x20004FFE = 0x30U;
       *(volatile uint8_t*)0x20004FFD = 0x40U;
       *(volatile uint8_t*)0x20004FFC = 0x50U;
       HAL_NVIC_SystemReset();
        //jumpToBootLoader();
      }
    }
    else
    {
      
    }
    HAL_IWDG_Refresh(&hiwdg);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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

/* USER CODE BEGIN 4 */

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
