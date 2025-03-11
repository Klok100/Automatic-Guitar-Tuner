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
#include <stdio.h>
#include <string.h>
#include "arm_math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_BUFFER_SIZE      256       // Size of the ADC sample buffer (FFT block size)
#define SAMPLE_RATE          10000     // ADC sampling rate in Hz (adjust to your needs)

// Global ADC buffer for DMA transfers. This buffer will be filled continuously.
volatile uint16_t adcBuffer[ADC_BUFFER_SIZE];
// Flag indicating that the ADC conversion (DMA transfer) is complete.
volatile uint8_t adcConvCompleteFlag = 0;

// FFT processing buffers (CMSIS-DSP requires float32 arrays)
float_t fftInput[ADC_BUFFER_SIZE];
float_t fftOutput[ADC_BUFFER_SIZE];

// FFT instance structure for fast real FFT as per CMSIS-DSP
arm_rfft_fast_instance_f32 fftInstance;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
UART_HandleTypeDef huart1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_ADC1_Init(void);
void Start_ADC_DMA(void);
void ProcessFFT(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

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
  /* USER CODE BEGIN 2 */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  /* USER CODE END 2 */

  /* Start ADC conversion using DMA. This call follows the HAL API naming conventions. */
  Start_ADC_DMA();

  /* Initialize the CMSIS-DSP FFT instance.
     The arm_rfft_fast_init_f32() function initializes the FFT instance for the given block size.
     Check return value for ARM_MATH_SUCCESS. */
  if (arm_rfft_fast_init_f32(&fftInstance, ADC_BUFFER_SIZE) != ARM_MATH_SUCCESS)
  {
    Error_Handler();
  }

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	    /* Wait for ADC DMA to fill the buffer */
	    if (adcConvCompleteFlag)
	    {
	      adcConvCompleteFlag = 0;

	      /* Convert ADC raw values (uint16_t) to floating-point data.
	         Scaling may be applied depending on your sensor calibration. */
	      for (uint32_t i = 0; i < ADC_BUFFER_SIZE; i++)
	      {
	        fftInput[i] = (float32_t)adcBuffer[i];
	      }

	      /* Process the FFT of the ADC data and control the motor accordingly. */
	      ProcessFFT();
	    }

	    /* Additional background tasks or a small delay can be added here */
	    HAL_Delay(10);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief  Start ADC in DMA mode.
  * @note   This function initiates the ADC1 conversion in DMA mode.
  *         The ADC results will be continuously transferred to the adcBuffer.
  * @retval None
  */
void Start_ADC_DMA(void)
{
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcBuffer, ADC_BUFFER_SIZE) != HAL_OK)
  {
    /* Start Error: Call error handler */
    Error_Handler();
  }
}

/**
  * @brief  Process ADC data with FFT and control motor.
  * @note   This function performs an FFT on fftInput and finds the dominant frequency.
  *         It then calls ControlMotor() to adjust the PWM output.
  */
void ProcessFFT(void)
{
  /* Execute the FFT: transform fftInput into fftOutput. The flag 0 indicates a forward FFT.
     The HAL-based code uses CMSIS-DSP API functions which follow UM2217 HAL API naming rules. */
  arm_rfft_fast_f32(&fftInstance, fftInput, fftOutput, 0);

  /* For real signals, the FFT is symmetric so we only need the first half of the bins. */
  uint32_t numBins = ADC_BUFFER_SIZE / 2;
  float32_t maxMagnitude = 0.0f;
  uint32_t maxIndex = 0;

  /* Use the CMSIS function arm_max_f32() to find the bin with the maximum magnitude.
     This function is in accordance with CMSIS-DSP conventions. */
  arm_max_f32(fftOutput, numBins, &maxMagnitude, &maxIndex);

  /* Convert the bin index to a frequency.
     Frequency resolution = SAMPLE_RATE / ADC_BUFFER_SIZE */
  float32_t peakFrequency = ((float32_t)maxIndex * SAMPLE_RATE) / ADC_BUFFER_SIZE;

  /* Print the peak frequency to the console using UART */
  char msg[50];
  sprintf(msg, "Peak Frequency: %.2f Hz\r\n", peakFrequency);
  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

/**
  * @brief  Initialize GPIO pins.
  * @note   Configures the ADC input pin (e.g., PA0) in analog mode and the LED output pin.
  *         Adjust pin names and port definitions to match your hardware.
  * @retval None
  */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable GPIO Clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure GPIO pin for ADC input.
     Assuming the ADC channel is connected to PA0. */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure GPIO pin for LED output.
     LED_GPIO_Port and LED_Pin should be defined in your project (e.g., in main.h or a board file). */
  //GPIO_InitStruct.Pin = LED_Pin; TODO
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-Pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  //HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct); TODO
}

/**
  * @brief  Initialize DMA controller for ADC1.
  * @note   This function enables the DMA1 clock, configures the DMA channel for ADC1,
  *         and associates the DMA handle with the ADC handle.
  *         It also configures the NVIC for DMA interrupts.
  * @retval None
  */
void MX_DMA_Init(void)
{
  /* Enable DMA1 clock */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure the DMA handle for ADC1.
     - Instance: DMA1_Stream0 (verify with your reference manual)
     - Request: DMA_REQUEST_ADC1 (specific to STM32H7 series)
     - Direction: Peripheral to Memory
     - Peripheral increment: Disabled (ADC data register remains constant)
     - Memory increment: Enabled (move through the buffer)
     - Data alignment: Half-word for both peripheral and memory (12-bit ADC data fits in a half-word)
     - Mode: Circular (for continuous data transfer)
     - Priority: High
     - FIFO mode: Disabled (typically FIFO is not needed for ADC DMA) */
  hdma_adc1.Instance = DMA1_Stream0;
  hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
  hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_adc1.Init.Mode = DMA_CIRCULAR;
  hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

  if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Associate the initialized DMA handle to the ADC handle */
  __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);

  /* Configure NVIC for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
}

/**
  * @brief  Initialize ADC1 peripheral.
  * @note   Configures ADC1 for continuous conversion in DMA mode.
  *         Follows HAL_ADC_Init() and HAL_ADC_ConfigChannel() as per UM2217 guidelines.
  */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;  // Adjust prescaler as needed
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  //hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  //hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure the ADC channel connected to the piezo sensor.
     Change ADC_CHANNEL_0 if your sensor is connected to a different channel. */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_387CYCLES_5;  // Longer sample time for stable readings
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /*AXI clock gating */
  RCC->CKGAENR = 0xFFFFFFFF;

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

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
