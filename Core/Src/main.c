/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  * PC0-2 are connected to the columns and PC3-6 are connected to the rows
  ******************************************************************************
  */


/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdlib.h>

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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */
void delay(void);
void keypad_init(void);
int keypad_kbhit(void);
void delayMs(int n);
int identifyDigit(int row, int value);
void toggleLED();
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

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* USER CODE BEGIN Init */
	keypad_init();
  /* USER CODE END Init */

  /* Configure the system clock */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */



	  int number = keypad_kbhit();

	  if(number == -1) {
		  continue;
	  }


	  for(int i = 0; i < number; i++)
	  {
		  toggleLED();
		  delayMs(500);
	  }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */


/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */


/* USER CODE BEGIN 4 */
void keypad_init(void)
{
	RCC->AHB1ENR |= 0x05; // enable clocks for GPIOA & GPIOC


	GPIOC->MODER &= ~0x0000ffff;
	GPIOC->MODER |= 0x00001540; // PC0-2 input, PC3-6 output
	GPIOC->PUPDR = 0x00000015; // enable pull up resistors

	GPIOA->MODER &= ~0x0000f00; // clear pins
	GPIOA->MODER |= 0x00000400; // set PA5 to output

	// set rows HIGH
	GPIOC->BSRR = 0x00000078;
}

int keypad_kbhit(void)
{
	/**
	 * Column mappings
	 * 6 (110) -> col 1
	 * 5 (101) -> col 2
	 * 3 (011) -> col 3
	 */
	int button = -1;
	char character = '-';
	int rowMasks[4] = {
		0x00080000, // drive row 1
		0x00100000, // drive row 2
		0x00200000, // drive row 3
		0x00400000 // drive row 4
	};

	for(int i = 0; i < 4; i++){
		GPIOC->BSRR = rowMasks[i];

		// read input register
		button = GPIOC->IDR & 0x00000007;

		// set rows high again
		GPIOC->BSRR = 0x00000078;

		if(button != 0x7) {
			character = identifyDigit(i + 1, button);
			return character;
		}
	}

	return -1;
}

int identifyDigit(int row, int value)
{
	switch(row) {
	case 1:
		 switch(value) {
		 case 6:
			 return 1;
			 break;

		 case 5:
			 return 2;
		 	break;

		 case 3:
			 return 3;
		 	break;
		 }
		break;

	case 2:
		switch(value) {
		 case 6:
			 return 4;
			 break;

		 case 5:
			 return 5;
			break;

		 case 3:
			 return 6;
			break;
		 }
		break;

	case 3:
		switch(value) {
		 case 6:
			 return 7;
			 break;

		 case 5:
			 return 8;
			break;

		 case 3:
			 return 9;
			break;
		 }
		break;

	case 4:
		switch(value) {
//		 case 6:
//			 return '*';
//			 break;

		 case 5:
			 return 0;
			break;

//		 case 3:
//			 return '#';
//			break;
		 }
		break;
	}

	return -1;
}


void delay(void)
{
	int i;
	for (i = 0; i < 500000; i++){
		for(int j = 0; j < 3192; j++ );
	}
}

void delayMs(int n)
{
	int i;
	for (; n > 0; n--)
		for (i = 0; i < 3195; i++) ;
}

void toggleLED()
{
	// toggle LED;
	GPIOA->BSRR = 0x00000020;
	delayMs(20);
	GPIOA->BSRR = 0x00200000;
	delayMs(20);
}

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
