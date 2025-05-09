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
#include <stdlib.h>

#define ARM_MATH_CM7
#include "arm_math.h"
#include "arm_const_structs.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define FFT_SIZE_V0_1 1024
#define FFT_SIZE_V2 64
#define SAMPLING_RATE 1024
#define TEST_VERSIONS 1
// TEST_VERSION 0 --> CFFT Piezo Input
// TEST_VERSION 1 --> RFFT Piezo Input
// TEST_VERSION 2 --> CFFT Sin Wave

#define NUM_STRINGS 6
#define MAX_PEAKS 18
#define FREQ_BIN_WIDTH 3
#define FREQ_TOLERANCE 7
#define HARM_TOLERANCE 2

#define E2_TUNE_TOL 0.5
#define A2_TUNE_TOL 0.75
#define D3_TUNE_TOL 1
#define G3_TUNE_TOL 1.35
#define B3_TUNE_TOL 1.7
#define E4_TUNE_TOL 2.3

#define E2 82.41
#define A2 110
#define D3 146.83
#define G3 196
#define B3 246.94
#define E4 329.63

#define E2_ADJUST 7
#define A2_ADJUST 9
#define D3_ADJUST 12
#define G3_ADJUST 16
#define B3_ADJUST 20
#define E4_ADJUST 27

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc3;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
char buffer[100] = "";

uint32_t adc_val = 0;
uint16_t adc_max_range = 65535;
float voltage = 0.0f;

float currVal = 0.0f;
float peakVal = 0.0f;
float currFreq = 0.0f;
float peakFreq = 0.0f;

uint16_t fftIndex = 0;
uint8_t fftFlag = 0;
uint8_t freqBin = 1;
uint16_t freqIndex = 0;

q15_t inputSignalV0[FFT_SIZE_V0_1];
q15_t complexOutputV0[FFT_SIZE_V0_1 * 2];
q15_t freqSpectrumV0[FFT_SIZE_V0_1];

float inputSignalV1[FFT_SIZE_V0_1];
float outputSignalV1[FFT_SIZE_V0_1];
float freqSpectrumV1[FFT_SIZE_V0_1 / 2];
float dBSpectrumV1[FFT_SIZE_V0_1 / 2];
float currFreqArray[FFT_SIZE_V0_1 / 2];

q15_t inputSignalV2[FFT_SIZE_V2];
q15_t complexOutputV2[FFT_SIZE_V2 * 2];
q15_t freqSpectrumV2[FFT_SIZE_V2];

arm_rfft_fast_instance_f32 fftHandler;

int takeFFTs = 1;
int inTuneFlag = 0;
int tuneCounter = 0;

float E2_low = 73.42;
float E2_high = 92.50;
float A2_low = 98;
float A2_high = 123.47;
float D3_low = 130.81;
float D3_high = 164.81;
float G3_low = 174.61;
float G3_high = 220;
float B3_low = 220.001;
float B3_high = 277.18;
float E4_low = 293.66;
float E4_high = 369.99;

int peaks_list_freq[MAX_PEAKS];
float peaks_list_val[MAX_PEAKS];
int E2_freq_bin[FREQ_BIN_WIDTH];
int A2_freq_bin[FREQ_BIN_WIDTH];
int D3_freq_bin[FREQ_BIN_WIDTH];
int G3_freq_bin[FREQ_BIN_WIDTH];
int B3_freq_bin[FREQ_BIN_WIDTH];
int E4_freq_bin[FREQ_BIN_WIDTH];

int E2_index = 0;
int A2_index = 0;
int D3_index = 0;
int G3_index = 0;
int B3_index = 0;
int E4_index = 0;

int E2_tune = 0;
int A2_tune = 0;
int D3_tune = 0;
int G3_tune = 0;
int B3_tune = 0;
int E4_tune = 0;
// 0 --> In Tune
// 1 --> Sharp
// 2 --> Flat

int prevDegreesRotated = 180;
int currDegreesRotated = 0;
float inTuneDiff = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC3_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	adc_val = HAL_ADC_GetValue(hadc);
	voltage = ((3300 * 2 * adc_val) / (float) adc_max_range) - 3300;

	if (TEST_VERSIONS == 0) { // --------------------------------------------------------------------------
		inputSignalV0[fftIndex] = voltage;
		complexOutputV0[fftIndex * 2] = voltage;
		complexOutputV0[fftIndex * 2 + 1] = 0;
		fftIndex++;

		if (fftIndex == FFT_SIZE_V0_1) {
			arm_cfft_q15(&arm_cfft_sR_q15_len64, &complexOutputV0[0], 0, 1);
			arm_cmplx_mag_q15(&complexOutputV0[0], &freqSpectrumV0[0], FFT_SIZE_V0_1);

			fftFlag = 1;
			fftIndex = 0;
		}
	} else if (TEST_VERSIONS == 1) { // -------------------------------------------------------------------
		if (takeFFTs == 1) {
			inputSignalV1[fftIndex] = voltage;
			fftIndex++;

			if (fftIndex == FFT_SIZE_V0_1) {
				arm_rfft_fast_f32(&fftHandler, &inputSignalV1[0], &outputSignalV1[0], 0);

				fftFlag = 1;
				fftIndex = 0;

				HAL_GPIO_TogglePin(GPIOE, Sampling_Rate_Pin);
			}
		}
	} else if (TEST_VERSIONS == 2) { // -------------------------------------------------------------------
		// Sin Wave Test Output
	}
}

void blinkGreenLED() {
	HAL_Delay(1000);
	HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_RESET);
}

void blinkYellowLED() {
	HAL_Delay(1000);
	HAL_GPIO_WritePin(Yellow_LED_GPIO_Port, Yellow_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(Yellow_LED_GPIO_Port, Yellow_LED_Pin, GPIO_PIN_RESET);
}

void blinkRedLED() {
	HAL_Delay(1000);
	HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_RESET);
}

void blinkAllLEDs() {
	HAL_Delay(1000);
	HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Yellow_LED_GPIO_Port, Yellow_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Yellow_LED_GPIO_Port, Yellow_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_RESET);
}

void resetFreqPeaksList() {
	for (int i = 0; i < MAX_PEAKS; i++) {
		peaks_list_freq[i] = 0;
		peaks_list_val[i] = 0;
	}
}

void resetFreqPeaksBins() {
	for (int i = 0; i < FREQ_BIN_WIDTH; i++) {
		E2_freq_bin[i] = 0;
		A2_freq_bin[i] = 0;
		D3_freq_bin[i] = 0;
		G3_freq_bin[i] = 0;
		B3_freq_bin[i] = 0;
		E4_freq_bin[i] = 0;
	}

	E2_index = 0;
	A2_index = 0;
	D3_index = 0;
	G3_index = 0;
	B3_index = 0;
	E4_index = 0;
}

void stopFFTsCheck() {
	if (E2_index > 0 &&
		A2_index > 0 &&
		D3_index > 0 &&
		G3_index > 0 &&
		B3_index > 0 &&
		E4_index > 0) {
		takeFFTs = 0;
	}
}

void clearBuffer() {
	memset(buffer, 0, sizeof(buffer));
}

void printFreqBins() {
	for (int i = 0; i < NUM_STRINGS; i++) {
		for (int j = 0; j < FREQ_BIN_WIDTH; j++) {
			if (i == 0) {
				if (E2_freq_bin[j] > 0) {
					snprintf(buffer, 100, "E2 Freq Bin: %d \r\n", E2_freq_bin[j]);
					HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
				}
			} else if (i == 1) {
				if (A2_freq_bin[j] > 0) {
					snprintf(buffer, 100, "A2 Freq Bin: %d \r\n", A2_freq_bin[j]);
					HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
				}
			} else if (i == 2) {
				if (D3_freq_bin[j] > 0) {
					snprintf(buffer, 100, "D3 Freq Bin: %d \r\n", D3_freq_bin[j]);
					HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
				}
			} else if (i == 3) {
				if (G3_freq_bin[j] > 0) {
					snprintf(buffer, 100, "G3 Freq Bin: %d \r\n", G3_freq_bin[j]);
					HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
				}
			} else if (i == 4) {
				if (B3_freq_bin[j] > 0) {
					snprintf(buffer, 100, "B3 Freq Bin: %d \r\n", B3_freq_bin[j]);
					HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
				}
			} else {
				if (E4_freq_bin[j] > 0) {
					snprintf(buffer, 100, "E4 Freq Bin: %d \r\n", E4_freq_bin[j]);
					HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
				}
			}
		}
	}

	clearBuffer();
}

void removeHarmonics() {
	int fundamental = 0;
	int harm_one = 0;
	int harm_two = 0;

	for (int i = 0; i < MAX_PEAKS; i++) {
		fundamental = peaks_list_freq[i];
		harm_one = fundamental * 2;
		harm_two = fundamental * 0; // ISSUE: Causes conflict with the upper strings right now

		for (int j = i + 1; j < MAX_PEAKS; j++) {
			if ((peaks_list_freq[j] != 0) &&
			   ((peaks_list_freq[j] >= harm_one - HARM_TOLERANCE && peaks_list_freq[j] <= harm_one + HARM_TOLERANCE) ||
				(peaks_list_freq[j] >= harm_two - HARM_TOLERANCE && peaks_list_freq[j] <= harm_two + HARM_TOLERANCE))) {
				peaks_list_freq[j] = 0;
			}
		}
	}

	clearBuffer();
}

void identifyFreqPeaks(float freq_vals[], int arr_length) {
	int curr_index = 0;
	resetFreqPeaksList();

	for (int i = 0; i < arr_length; i++) {
		if (freq_vals[i] >= 30000 && i > 70) {
			if (curr_index > 0) {
				if ((peaks_list_val[curr_index - 1] < freq_vals[i]) && (i - peaks_list_freq[curr_index - 1] <= FREQ_TOLERANCE)) {
					peaks_list_freq[curr_index - 1] = i;
					peaks_list_val[curr_index - 1] = freq_vals[i];
				} else if ((peaks_list_val[curr_index - 1] > freq_vals[i]) && (i - peaks_list_freq[curr_index - 1] <= FREQ_TOLERANCE)) {
					continue;
				} else {
					peaks_list_freq[curr_index] = i;
					peaks_list_val[curr_index] = freq_vals[i];
					curr_index++;
				}
			} else {
				peaks_list_freq[curr_index] = i;
				peaks_list_val[curr_index] = freq_vals[i];
				curr_index++;
			}
		}
	}
}

int determineDuplicateFreqs(int freqs[], int new_freq) {
	for (int i = 0; i < FREQ_BIN_WIDTH; i++) {
		if (freqs[i] == new_freq) {
			return 1;
		}
	}

	return 0;
}

void determineFreqBins() {
	int curr_peak = 0;
	for (int i = 0; i < MAX_PEAKS; i++) {
		curr_peak = peaks_list_freq[i];
		if ((curr_peak + E2_ADJUST > E2_low && curr_peak + E2_ADJUST < E2_high) &&
			(determineDuplicateFreqs(&E2_freq_bin[0], curr_peak + E2_ADJUST) == 0) &&
			(E2_index < FREQ_BIN_WIDTH)) {
			E2_freq_bin[E2_index] = curr_peak + E2_ADJUST;
			E2_index++;
		} else if ((curr_peak + A2_ADJUST > A2_low && curr_peak + A2_ADJUST < A2_high) &&
				   (determineDuplicateFreqs(&A2_freq_bin[0], curr_peak + A2_ADJUST) == 0) &&
				   (A2_index < FREQ_BIN_WIDTH)) {
			A2_freq_bin[A2_index] = curr_peak + A2_ADJUST;
			A2_index++;
		} else if ((curr_peak + D3_ADJUST > D3_low && curr_peak + D3_ADJUST < D3_high) &&
				   (determineDuplicateFreqs(&D3_freq_bin[0], curr_peak + D3_ADJUST) == 0) &&
				   (D3_index < FREQ_BIN_WIDTH)) {
			D3_freq_bin[D3_index] = curr_peak+ D3_ADJUST;
			D3_index++;
		} else if ((curr_peak + G3_ADJUST > G3_low && curr_peak + G3_ADJUST < G3_high) &&
				   (determineDuplicateFreqs(&G3_freq_bin[0], curr_peak + G3_ADJUST) == 0) &&
				   (G3_index < FREQ_BIN_WIDTH)) {
			G3_freq_bin[G3_index] = curr_peak+ G3_ADJUST;
			G3_index++;
		} else if ((curr_peak + B3_ADJUST > B3_low && curr_peak + B3_ADJUST < B3_high) &&
				   (determineDuplicateFreqs(&B3_freq_bin[0], curr_peak + B3_ADJUST) == 0) &&
				   (B3_index < FREQ_BIN_WIDTH)) {
			B3_freq_bin[B3_index] = curr_peak+ B3_ADJUST ;
			B3_index++;
		} else if ((curr_peak + E4_ADJUST > E4_low && curr_peak + E4_ADJUST < E4_high) &&
				   (determineDuplicateFreqs(&E4_freq_bin[0], curr_peak + E4_ADJUST) == 0) &&
				   (E4_index < FREQ_BIN_WIDTH)) {
			E4_freq_bin[E4_index] = curr_peak+ E4_ADJUST ;
			E4_index++;
		} else {
			continue;
		}
	}
}

void checkClosestFreq(int freqs[], float correct_note) {
	int peakOneFreqDist = abs(correct_note - freqs[0]);
	int peakTwoFreqDist = abs(correct_note - freqs[1]);
	int peakThreeFreqDist = abs(correct_note - freqs[2]);

	if (peakOneFreqDist <= peakTwoFreqDist && peakOneFreqDist <= peakThreeFreqDist) {
		freqs[0] = freqs[0];
		freqs[1] = 0;
		freqs[2] = 0;
	} else if (peakTwoFreqDist <= peakOneFreqDist && peakTwoFreqDist <= peakThreeFreqDist) {
		freqs[0] = freqs[1];
		freqs[1] = 0;
		freqs[2] = 0;
	} else if (peakThreeFreqDist <= peakOneFreqDist && peakThreeFreqDist <= peakTwoFreqDist) {
		freqs[0] = freqs[2];
		freqs[1] = 0;
		freqs[2] = 0;
	}
}

void cleanFreqBins() {
	for (int i = 0; i < NUM_STRINGS; i++) {
		if (i == 0) {
			if (E2_index > 1) {
				checkClosestFreq(&E2_freq_bin[0], E2);
				E2_index = 1;
			}
		} else if (i == 1) {
			if (A2_index > 1) {
				checkClosestFreq(&A2_freq_bin[0], A2);
				A2_index = 1;
			}
		} else if (i == 2) {
			if (D3_index > 1) {
				checkClosestFreq(&D3_freq_bin[0], D3);
				D3_index = 1;
			}
		} else if (i == 3) {
			if (G3_index > 1) {
				checkClosestFreq(&G3_freq_bin[0], G3);
				G3_index = 1;
			}
		} else if (i == 4) {
			if (B3_index > 1) {
				checkClosestFreq(&B3_freq_bin[0], B3);
				B3_index = 1;
			}
		} else if (i == 5) {
			if (E4_index > 1) {
				checkClosestFreq(&E4_freq_bin[0], E4);
				E4_index = 1;
			}
		}
	}
}

void rotateMotor(int direction, int duration){
	if (direction == 1) {
		HAL_GPIO_WritePin(Motor_Control_1_GPIO_Port, Motor_Control_1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, Motor_Control_2_Pin, GPIO_PIN_RESET);
	} else if (direction == 2) {
		HAL_GPIO_WritePin(Motor_Control_1_GPIO_Port, Motor_Control_1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, Motor_Control_2_Pin, GPIO_PIN_SET);
	}

	HAL_GPIO_WritePin(Motor_Enable_GPIO_Port, Motor_Enable_Pin, GPIO_PIN_SET);

	snprintf(buffer, 100, "Motor Rotation Duration: %d \r\n", duration);
	HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
	clearBuffer();

	HAL_Delay(duration);


	HAL_GPIO_WritePin(Motor_Control_1_GPIO_Port, Motor_Control_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, Motor_Control_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Enable_GPIO_Port, Motor_Enable_Pin, GPIO_PIN_RESET);
}

int determineDuration(float correctFreq, float currFreq){
	inTuneDiff = abs(correctFreq - currFreq);
	currDegreesRotated = (inTuneDiff / 21) * 180;
	if (currDegreesRotated < prevDegreesRotated) {
		prevDegreesRotated = currDegreesRotated;
		return (currDegreesRotated / 3) * 1000;
	} else {
		return -1;
	}
}

void turnMotors() {
	if (E2_tune == 1) {
		snprintf(buffer, 100, "E2 String is Sharp... Turning Motor Clockwise \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
	} else if (E2_tune == 2) {
		snprintf(buffer, 100, "E2 String is Flat... Turning Motor Counterclockwise \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
	}
	clearBuffer();

	if (A2_tune == 1) {
		snprintf(buffer, 100, "A2 String is Sharp... Turning Motor Clockwise \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
	} else if (A2_tune == 2) {
		snprintf(buffer, 100, "A2 String is Flat... Turning Motor Counterclockwise \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
	}
	clearBuffer();

	if (D3_tune == 1) {
		snprintf(buffer, 100, "D3 String is Sharp... Turning Motor Clockwise \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
	} else if (D3_tune == 2) {
		snprintf(buffer, 100, "D3 String is Flat... Turning Motor Counterclockwise \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
	}
	clearBuffer();

	if (G3_tune == 1) {
		snprintf(buffer, 100, "G3 String is Sharp... Turning Motor Clockwise \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
		clearBuffer();

		rotateMotor(1, determineDuration(G3, G3_freq_bin[0]));
	} else if (G3_tune == 2) {
		snprintf(buffer, 100, "G3 String is Flat... Turning Motor Counterclockwise \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
		clearBuffer();

		rotateMotor(2, determineDuration(G3, G3_freq_bin[0]));
	}
	clearBuffer();

	if (B3_tune == 1) {
		snprintf(buffer, 100, "B3 String is Sharp... Turning Motor Clockwise \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
	} else if (B3_tune == 2) {
		snprintf(buffer, 100, "B3 String is Flat... Turning Motor Counterclockwise \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
	}
	clearBuffer();

	if (E4_tune == 1) {
		snprintf(buffer, 100, "E4 String is Sharp... Turning Motor Clockwise \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
	} else if (E4_tune == 2) {
		snprintf(buffer, 100, "E4 String is Flat... Turning Motor Counterclockwise \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
	}
	clearBuffer();
}

void isFlat() {
	blinkRedLED();
}

void isSharp() {
	blinkYellowLED();
}

void isInTune() {
	blinkGreenLED();
}

int calculateTuning(float curr_freq, float correct_freq, float tune_tol) {
	if (curr_freq >= correct_freq - tune_tol && curr_freq <= correct_freq + tune_tol) {
		//isInTune();
		return 0;
	} else if (curr_freq > correct_freq + tune_tol) {
		//isSharp();
		return 1;
	} else if (curr_freq < correct_freq - tune_tol) {
		//isFlat();
		return 2;
	} else {
		HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Yellow_LED_GPIO_Port, Yellow_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_SET);
		return 3;
	}
}

void checkTuning() {
	inTuneFlag = 0;
	for (int i = 0; i < NUM_STRINGS; i++) {
		if (i == 0) {
			E2_tune = calculateTuning(E2_freq_bin[0], E2, E2_TUNE_TOL);
			inTuneFlag += E2_tune;
		} else if (i == 1) {
			A2_tune = calculateTuning(A2_freq_bin[0], A2, A2_TUNE_TOL);
			inTuneFlag += A2_tune;
		} else if (i == 2) {
			D3_tune = calculateTuning(D3_freq_bin[0], D3, D3_TUNE_TOL);
			inTuneFlag += D3_tune;
		} else if (i == 3) {
			G3_tune = calculateTuning(G3_freq_bin[0], G3, G3_TUNE_TOL);
			inTuneFlag += G3_tune;
		} else if (i == 4) {
			B3_tune = calculateTuning(B3_freq_bin[0], B3, B3_TUNE_TOL);
			inTuneFlag += B3_tune;
		} else {
			E4_tune = calculateTuning(E4_freq_bin[0], E4, E4_TUNE_TOL);
			inTuneFlag += E4_tune;
		}
	}

	if (inTuneFlag == 0) {
		snprintf(buffer, 100, "Your guitar is in tune and ready to play! \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
		clearBuffer();

		takeFFTs = 2;
	} else {
		snprintf(buffer, 100, "Tuning still in process... \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
		clearBuffer();

		turnMotors();

		snprintf(buffer, 100, "Please strum again! \r\n");
		HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
		clearBuffer();

		takeFFTs = 1;
	}
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
  MX_GPIO_Init();
  MX_ADC3_Init();
  MX_TIM1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  arm_rfft_fast_init_f32(&fftHandler, FFT_SIZE_V0_1);

  HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET_LINEARITY, ADC_DIFFERENTIAL_ENDED);
  HAL_TIM_Base_Start(&htim1);
  HAL_ADC_Start_IT(&hadc3);

  snprintf(buffer, 100, "Please strum your guitar to begin the tuning process! \r\n");
  HAL_UART_Transmit(&huart3, (uint8_t*) buffer, 100, 100);
  clearBuffer();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	blinkAllLEDs();

	if (TEST_VERSIONS == 1) {
		if (fftFlag == 1 && takeFFTs == 1) {
			freqIndex = 0;
			for (int i = 0; i < FFT_SIZE_V0_1; i += 2) {
				currVal = sqrtf((outputSignalV1[i] * outputSignalV1[i]) + (outputSignalV1[i + 1] * outputSignalV1[i + 1]));
				currFreq = (freqIndex * SAMPLING_RATE) / (float) FFT_SIZE_V0_1;

				currFreqArray[i / 2] = currFreq;
				freqSpectrumV1[i / 2] = currVal;
				dBSpectrumV1[i / 2] = 20 * log10(currVal);

				freqIndex++;
			}

			identifyFreqPeaks(&freqSpectrumV1[0], FFT_SIZE_V0_1 / 2);

			removeHarmonics();

			determineFreqBins();

			stopFFTsCheck();

			fftFlag = 0;

		} else if (takeFFTs == 0) {

			cleanFreqBins();

			printFreqBins();

			checkTuning();

			resetFreqPeaksBins();
		} else if (takeFFTs == 2) {
			HAL_Delay(5000);

			takeFFTs = 1;
		}
	} else if (TEST_VERSIONS == 2) {
		for (int i = 0; i < FFT_SIZE_V2; i++) {
			inputSignalV2[i] = round(32767 * sin(freqBin * 2.0 * M_PI * i / FFT_SIZE_V2));
			complexOutputV2[i * 2] = inputSignalV2[i];
			complexOutputV2[i * 2 + 1] = 0;
		}

		freqBin++;
		if (freqBin >= FFT_SIZE_V2 / 2) {
			freqBin = 1;
		}

		arm_cfft_q15(&arm_cfft_sR_q15_len64, &complexOutputV2[0], 0, 1);
		arm_cmplx_mag_q15(&complexOutputV2[0], &freqSpectrumV2[0], FFT_SIZE_V2);

	}

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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.Resolution = ADC_RESOLUTION_16B;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T1_TRGO;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc3.Init.OversamplingMode = DISABLE;
  hadc3.Init.Oversampling.Ratio = 1;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_DIFFERENTIAL_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

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
  htim1.Init.Prescaler = 24000-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 9.765625-1;
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Motor_Enable_GPIO_Port, Motor_Enable_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Green_LED_Pin|Red_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, Motor_Control_2_Pin|Sampling_Rate_Pin|Yellow_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Motor_Control_1_GPIO_Port, Motor_Control_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Motor_Enable_Pin */
  GPIO_InitStruct.Pin = Motor_Enable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Motor_Enable_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Green_LED_Pin Red_LED_Pin */
  GPIO_InitStruct.Pin = Green_LED_Pin|Red_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Motor_Control_2_Pin Sampling_Rate_Pin Yellow_LED_Pin */
  GPIO_InitStruct.Pin = Motor_Control_2_Pin|Sampling_Rate_Pin|Yellow_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : Motor_Control_1_Pin */
  GPIO_InitStruct.Pin = Motor_Control_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Motor_Control_1_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
