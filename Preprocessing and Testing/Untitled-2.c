/* USER CODE BEGIN Header */
/**
  **************************
  * @file           : main.c
  * @brief          : Main program body
  **************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CS_Pin GPIO_PIN_6  // Example pin number, replace with your actual pin number
#define RESET_Pin GPIO_PIN_10  // Example pin number, replace with your actual pin number
#define START_Pin GPIO_PIN_8  // Example pin number, replace with your actual pin number
#define DRDY_Pin GPIO_PIN_4  // Example pin number, replace with your actual pin number
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

#define SYSTICKCLOCK 16000000ULL // System clock frequency (8MHz)
#define SYSTICKPERUS (SYSTICKCLOCK / 1000000UL) // SysTick timer counts per microsecond

static void inline _attribute_((always_inline)) delay_us(unsigned delay) {
    uint32_t ticks = SYSTICKPERUS * delay;
    uint32_t start_tick = SysTick->VAL;

    while(SysTick->VAL - start_tick < ticks);
}

int _write(int le, char *ptr, int len){
	int DataIdx;
	for(DataIdx = 0; DataIdx < len; DataIdx++){
			ITM_SendChar(*ptr++);
	}
	return len;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void write_register_byte(uint8_t reg_addr, uint8_t val_hex){
   HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
   uint8_t adress = 0x40|reg_addr;
   uint8_t dummy1 = 0x00;
   HAL_SPI_Transmit(&hspi1, (uint8_t*)&adress, 1, 0x1000);
   HAL_SPI_Transmit(&hspi1, (uint8_t*)&dummy1, 1, 0x1000);
   HAL_SPI_Transmit(&hspi1, (uint8_t*)&val_hex, 1, 0x1000);
//   delay_us(100);
   HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);
}

uint8_t read_register(uint8_t reg_addr){
	HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
	uint8_t out;
	uint8_t adress = 0x20 | reg_addr;
	uint8_t dummy1 = 0x00;
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&adress, 1, 0x1000);
		delay_us(10);//delay 4 t_clk
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&dummy1, 1, 0x1000);
		delay_us(10);//delay 4 t_clk
	HAL_SPI_Receive(&hspi1, (uint8_t *)&out, 1, 0x1000);    //MISO
//	delay_us(2);
	delay_us(100);
	HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);
	return(out);
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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

    uint8_t sdatac = 0x11;
	uint8_t start = 0x08;
	uint8_t rdatac = 0x10;
	uint8_t rdata = 0x12;
	uint8_t dummy = 0x00;
	uint8_t device;
	uint8_t rst = 0x06;

	uint8_t id = 0x00;
	uint8_t config1 = 0x01;
	uint8_t config2 = 0x02;
	uint8_t config3 = 0x03;
	uint8_t loff = 0x04;
	uint8_t ch1set = 0x05;
	uint8_t ch2set = 0x06;
	uint8_t ch3set = 0x07;
	uint8_t ch4set = 0x08;
	uint8_t ch5set = 0x09;
	uint8_t ch6set = 0x0A;
	uint8_t ch7set = 0x0B;
	uint8_t ch8set = 0x0C;
	uint8_t bias_sensp = 0x0D;
	uint8_t bias_sensn = 0x0E;
	uint8_t loff_sensp = 0x0F;
	uint8_t loff_sensn = 0x10;
	uint8_t loff_flip = 0x11;
	uint8_t loff_statp = 0x12;
	uint8_t loff_statn = 0x13;
	uint8_t gpio = 0x14;
	uint8_t misc1 = 0x15;
	uint8_t misc2 = 0x16;
	uint8_t config4 = 0x17;

  	uint32_t status_reg;
  	uint8_t inbyte;
  	status_reg = 0x00000000;
  	uint32_t output[8];
  	uint32_t dataPacket = 0x00000000;
  	uint8_t dataByte;
  	int z = 0;
  	int c = 6;


  	HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
  	delay_us(10);
  	HAL_SPI_Transmit(&hspi1, (uint8_t*)&rst, 1, 0x1000);
  	delay_us(10);
  	HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);
  	delay_us(10000);


  	HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
  	delay_us(10);
  	HAL_SPI_Transmit(&hspi1, (uint8_t *)&sdatac, 1, 0x1000);
  	delay_us(10);
  	HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);
  	delay_us(10000);


  	//Register Configurations
	write_register_byte(config3, 0xE0);//11111000
	delay_us(10000);
	write_register_byte(config1, 0xD6);//11010110
	write_register_byte(config2, 0xD0);//11010000
	write_register_byte(loff, 0x00);//11100000
	write_register_byte(ch1set, 0x65);//01001101
	write_register_byte(ch2set, 0x65);
	write_register_byte(ch3set, 0x65);
	write_register_byte(ch4set, 0x65);
	write_register_byte(ch5set, 0x65);
	write_register_byte(ch6set, 0x65);
	write_register_byte(ch7set, 0x65);
	write_register_byte(ch8set, 0x65);
	write_register_byte(bias_sensp, 0x03);//00000001
	write_register_byte(bias_sensn, 0x00);//00000001
	write_register_byte(loff_sensp, 0x00);
	write_register_byte(loff_sensn, 0x00);
	write_register_byte(loff_flip, 0x00);
	//	read loff_statp
	//	read loff_statn
	write_register_byte(gpio, 0x0F);
	write_register_byte(misc1, 0x20);//00100000
	write_register_byte(misc2, 0x00);
	write_register_byte(config4, 0x00);//00000000


  	HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
  	delay_us(10);
  	HAL_SPI_Transmit(&hspi1, (uint8_t *)&sdatac, 1, 0x1000);
  	delay_us(10);
  	HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);
  	delay_us(10000);

  	device = read_register(id);
  	uint8_t dev_id[] = "Device Id is : ";
  	HAL_UART_Transmit(&huart2, dev_id, sizeof(dev_id) - 1, 0x1000); // -1 to exclude null terminator
  	char ads[16];
  	sprintf(ads, "%u\n", device); // Dereference the pointer to get the value
  	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
    	printf(dev_id, "\n");
    	printf(ads, "\n");
  	delay_us(1000);


  //	device = read_register(loff_statp);
  //	uint8_t dev_id1[] = "LOFF_STATP is : ";
  //	HAL_UART_Transmit(&huart2, dev_id1, sizeof(dev_id1) - 1, 0x1000); // -1 to exclude null terminator
  //	sprintf(ads, "%u\n", device); // Dereference the pointer to get the value
  //	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
  ////  	printf(dev_id1, "\n");
  //  	printf(ads, "\n");
  //	delay_us(1000);
  //
  //
  //	device = read_register(loff_statn);
  //	uint8_t dev_id2[] = "LOFF_STATN is : ";
  //	HAL_UART_Transmit(&huart2, dev_id2, sizeof(dev_id2) - 1, 0x1000); // -1 to exclude null terminator
  //	sprintf(ads, "%u\n", device); // Dereference the pointer to get the value
  //	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
  ////  	printf(dev_id2, "\n");
  //  	printf(ads, "\n");
  //	delay_us(1000);

  	device = read_register(config1);
  	uint8_t dev_id3[] = "Config 1 is : ";
  	HAL_UART_Transmit(&huart2, dev_id3, sizeof(dev_id3) - 1, 0x1000); // -1 to exclude null terminator
  	sprintf(ads, "%u\n", device); // Dereference the pointer to get the value
  	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
	printf(dev_id3, "\n");
	printf(ads, "\n");
  	delay_us(1000);


  	HAL_GPIO_WritePin(GPIOA, START_Pin, GPIO_PIN_SET);
	delay_us(10000);

  	//send START command
  	HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
  	delay_us(10);
  	HAL_SPI_Transmit(&hspi1, (uint8_t *)&start, 1, 0x1000);
  	delay_us(10);
  	HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);
  	delay_us(10000);

//	  send RDATAC command
//  	HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
//  	delay_us(4);
//  	HAL_SPI_Transmit(&hspi1, (uint8_t *)&rdatac, 1, 0x1000);
//  	delay_us(1000);
//  	HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//  		HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
//  		delay_us(10);
//  		HAL_SPI_Transmit(&hspi1, (uint8_t *)&rdata, 1, 0x1000);
//  		delay_us(10);
//  		HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_SET);
//  		delay_us(100);

//		c--;
//		if (HAL_GPIO_ReadPin(GPIOB, DRDY_Pin) == 0){
//			z = 1;
//		}
		if (HAL_GPIO_ReadPin(GPIOB, DRDY_Pin) == 0){
//			printf("success");
//			z = 0;
			HAL_GPIO_WritePin(GPIOB, CS_Pin, GPIO_PIN_RESET);
			for (int j = 0; j < 3; j++) {
				HAL_SPI_Receive(&hspi1, (uint8_t*)&inbyte, 1, 0x1000); // Pass the address of inbyte
				status_reg = (status_reg << 8) | inbyte;
			}
//			delay_us(10);
			for(int i = 0; i < 8; i++) {
				for(int j = 0; j < 3; j++) {
					HAL_SPI_Receive(&hspi1, (uint8_t*)&dataByte, 1, 0x1000); // Corrected variable name
					dataPacket = (dataPacket << 8) | dataByte;
				}
				output[i] = dataPacket;
				dataPacket = 0;
			}
			delay_us(100);


			char ads_data[16];
			sprintf(ads_data, "%lu\n", status_reg);
			printf("Status_reg: %s", ads_data);
			HAL_UART_Transmit(&huart2, (uint8_t*)ads_data, strlen(ads_data), 0x1000);
			for(int i = 0; i < 8; i++) {
				 sprintf(ads_data, "%lu\n", output[i]);
				 printf("Channel : %s", ads_data);
				 HAL_UART_Transmit(&huart2, (uint8_t*)ads_data, strlen(ads_data), 0x1000);

			}
			delay_us(100);
		}
		else{
			continue;
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB10 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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