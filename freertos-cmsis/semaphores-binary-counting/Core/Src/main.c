/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef hlpuart1;

/* Definitions for LPT */
osThreadId_t LPTHandle;
const osThreadAttr_t LPT_attributes = {
  .name = "LPT",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for MPT */
osThreadId_t MPTHandle;
const osThreadAttr_t MPT_attributes = {
  .name = "MPT",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for HPT */
osThreadId_t HPTHandle;
const osThreadAttr_t HPT_attributes = {
  .name = "HPT",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for VHPT */
osThreadId_t VHPTHandle;
const osThreadAttr_t VHPT_attributes = {
  .name = "VHPT",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for myBinarySem01 */
osSemaphoreId_t myBinarySem01Handle;
const osSemaphoreAttr_t myBinarySem01_attributes = {
  .name = "myBinarySem01"
};
/* Definitions for myCountingSem01 */
osSemaphoreId_t myCountingSem01Handle;
const osSemaphoreAttr_t myCountingSem01_attributes = {
  .name = "myCountingSem01"
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
void StartLPT(void *argument);
void StartMPT(void *argument);
void StartHPT(void *argument);
void StartVHPT(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int __io_putchar (int ch)
{
	uint8_t c = ch;
	HAL_UART_Transmit(&hlpuart1, &c, 1, HAL_MAX_DELAY);
	return ch;
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
  MX_LPUART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of myBinarySem01 */
  myBinarySem01Handle = osSemaphoreNew(1, 1, &myBinarySem01_attributes);

  /* creation of myCountingSem01 */
  myCountingSem01Handle = osSemaphoreNew(3, 3, &myCountingSem01_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LPT */
  LPTHandle = osThreadNew(StartLPT, NULL, &LPT_attributes);

  /* creation of MPT */
  MPTHandle = osThreadNew(StartMPT, NULL, &MPT_attributes);

  /* creation of HPT */
  HPTHandle = osThreadNew(StartHPT, NULL, &HPT_attributes);

  /* creation of VHPT */
  VHPTHandle = osThreadNew(StartVHPT, NULL, &VHPT_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

int resource[3] = {111,222,333};

char *resourceOwner[3] = {"Free", "Free", "Free"};

int getResource(char *taskName)
{
    for(int i=0;i<3;i++)
    {
        if(strcmp(resourceOwner[i], "Free")==0)
        {
            resourceOwner[i] = taskName;
            return i;
        }
    }

    return -1;
}

void releaseResource(int id)
{
    resourceOwner[id] = "Free";
}

void printResourceTable(void)
{
    printf("\nResource Table:\n");

    for(int i=0;i<3;i++)
    {
        printf("Resource %d -> %s\n", resource[i], resourceOwner[i]);
    }

    printf("\n");
}



/* LPT Task for Binary Sem */
// void StartLPT(void *argument)
// {
//   /* USER CODE BEGIN 5 */
// 	uint32_t wait;
//   /* Infinite loop */
//   for(;;)
//   {
// 	  printf ("Entered LPT\n\n");
// 	  osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
// 	  printf ("LPT Using Resource\n\n");
// 	  wait = 50000000;
// 	  while (wait--);
// 	  printf ("LPT Finished, released Semaphore\n\n");
// 	  osSemaphoreRelease(myBinarySem01Handle);
// 	  printf ("LPT going to Sleep\n\n");
// 	  osDelay(500);
//   }
//   /* USER CODE END 5 */
// }
/* LPT Task for Binary Sem */


/* MPT Task for Binary Sem */
// void StartMPT(void *argument)
// {
//   /* USER CODE BEGIN StartMPT */
//   /* Infinite loop */
//   for(;;)
//   {
// 	  printf ("Entered MPT\n\n");
// 	  osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
// 	  printf ("MPT completed Task, released Semaphore\n\n");
// 	  osSemaphoreRelease(myBinarySem01Handle);
// 	  printf ("MPT going to Sleep\n\n");
// 	  osDelay(200);
//   }
//   /* USER CODE END StartMPT */
// }
/* MPT Task for Binary Sem */


/* HPT Task for Binary Sem */
// void StartHPT(void *argument)
// {
//   /* USER CODE BEGIN StartHPT */
//   /* Infinite loop */
//   for(;;)
//   {
//         printf ("Inside HPT\n\n");
//         osDelay(1000);
//   }
//   /* USER CODE END StartHPT */
// }
/* HPT Task for Binary Sem */






/* LPT Task for Priority Inversion */
// void StartLPT(void *argument)
// {
//   /* USER CODE BEGIN 5 */
// 	osSemaphoreRelease(myBinarySem01Handle);
// 	uint32_t wait;
//   /* Infinite loop */
//   for(;;)
//   {
// 	  printf ("Entered LPT, waiting for Semaphore\n\n");
// 	  osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
// 	  printf ("LPT Acquired Semaphore, using Resource\n\n");
//       wait = 10000000;
// 	  while (wait--);
// 	  printf ("LPT finished, released Semaphore\n\n");
// 	  osSemaphoreRelease(myBinarySem01Handle);
// 	  printf ("LPT going to Sleep\n\n");
// 	  osDelay(500);
//   }
//   /* USER CODE END 5 */
// }
/* LPT Task for Priority Inversion */


/* MPT Task for Priority Inversion */
// void StartMPT(void *argument)
// {
//   /* USER CODE BEGIN StartMPT */
// 	uint32_t wait;
//   /* Infinite loop */
//   for(;;)
//   {
// 	  printf ("Entered MPT, performing task\n\n");
// 	  wait = 50000000;
// 	  while (wait--);
// 	  printf ("MPT finished, going to Sleep\n\n");
// 	  osDelay(500);
//   }
//   /* USER CODE END StartMPT */
// }
/* MPT Task for Priority Inversion */


/* HPT Task for Priority Inversion */
// void StartHPT(void *argument)
// {
//   /* USER CODE BEGIN StartHPT */
// 
//   /* Infinite loop */
//   for(;;)
//   {
// 	  printf ("Entered HPT, waiting for Semaphore\n\n");
// 	  osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
// 	  printf ("HPT Acquired and now releasing Semaphore\n\n");
// 	  osSemaphoreRelease(myBinarySem01Handle);
// 	  printf ("HPT going to Sleep\n\n");
// 	  osDelay(200);
//   }
//   /* USER CODE END StartHPT */
// }
/* HPT Task for Priority Inversion */

/* USER CODE END 4 */

/* LPT Task for Priority Inversion */
void StartLPT(void *argument)
{
  /* USER CODE BEGIN 5 */
	int resID;
  /* Infinite loop */
  for(;;)
  {
      printf("LPT waiting for resource\n");

      osSemaphoreAcquire(myCountingSem01Handle, osWaitForever);

      resID = getResource("LPT");

      printf("LPT accessing resource %d\n", resource[resID]);

      printResourceTable();

      osDelay(2000);

      printf("LPT finished using resource\n");

      releaseResource(resID);

      osSemaphoreRelease(myCountingSem01Handle);

      osDelay(1000);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartMPT */
/**
* @brief Function implementing the MPT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMPT */
void StartMPT(void *argument)
{
  /* USER CODE BEGIN StartMPT */
	int resID;
  /* Infinite loop */
  for(;;)
  {
      printf("MPT waiting for resource\n");

      osSemaphoreAcquire(myCountingSem01Handle, osWaitForever);

      resID = getResource("MPT");

      printf("MPT accessing resource %d\n", resource[resID]);

      printResourceTable();

      osDelay(2000);

      printf("MPT finished using resource\n");

      releaseResource(resID);

      osSemaphoreRelease(myCountingSem01Handle);

      osDelay(1000);
  }
  /* USER CODE END StartMPT */
}

/* USER CODE BEGIN Header_StartHPT */
/**
* @brief Function implementing the HPT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartHPT */
void StartHPT(void *argument)
{
  /* USER CODE BEGIN StartHPT */
	int resID;
  /* Infinite loop */
  for(;;)
  {
      printf("HPT waiting for resource\n");

      osSemaphoreAcquire(myCountingSem01Handle, osWaitForever);

      resID = getResource("HPT");

      printf("HPT accessing resource %d\n", resource[resID]);

      printResourceTable();

      osDelay(2000);

      printf("HPT finished using resource\n");

      releaseResource(resID);

      osSemaphoreRelease(myCountingSem01Handle);

      osDelay(1000);
  }
  /* USER CODE END StartHPT */
}

/* USER CODE BEGIN Header_StartVHPT */
/**
* @brief Function implementing the VHPT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartVHPT */
void StartVHPT(void *argument)
{
  /* USER CODE BEGIN StartVHPT */
	int resID;
  /* Infinite loop */
  for(;;)
  {
      printf("VHPT waiting for resource\n");

      osSemaphoreAcquire(myCountingSem01Handle, osWaitForever);

      resID = getResource("VHPT");

      printf("VHPT accessing resource %d\n", resource[resID]);

      printResourceTable();

      osDelay(2000);

      printf("VHPT finished using resource\n");

      releaseResource(resID);

      osSemaphoreRelease(myCountingSem01Handle);

      osDelay(1000);
  }
  /* USER CODE END StartVHPT */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
#ifdef USE_FULL_ASSERT
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
