/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
//#define CS_Pin GPIO_PIN_6  // Example pin number, replace with your actual pin number
//#define RESET_Pin GPIO_PIN_7  // Example pin number, replace with your actual pin number
//#define DRDY_Pin GPIO_PIN_9  // Example pin number, replace with your actual pin number
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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

#define SYSTICKCLOCK 16000000ULL // System clock frequency (8MHz)
#define SYSTICKPERUS (SYSTICKCLOCK / 1000000UL) // SysTick timer counts per microsecond

static void delay_us(unsigned delay) __attribute__((always_inline));

static void __attribute__((always_inline)) delay_us(unsigned delay) {
    uint32_t ticks = SYSTICKPERUS * delay;
    uint32_t start_tick = SysTick->VAL;

    while(SysTick->VAL - start_tick < ticks);
}
//__attribute__((always_inline))
void delay_try(unsigned delay){
	int i=0;
	for(i=0; i<delay; i++){
		__NOP();
	}
}
uint8_t rreg_command(uint8_t buffer1, uint8_t buffer2){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	uint8_t device;
	  delay_try(10);
	  HAL_SPI_Transmit(&hspi1, (uint8_t *)&buffer1, 1, 0x1000);   //MOSI
	  delay_try(2);//delay 4 t_clk
	  HAL_SPI_Transmit(&hspi1, (uint8_t *)&buffer2, 1, 0x1000);  //MOSI
	  delay_try(2);//delay 4 t_clk
	  HAL_SPI_Receive(&hspi1, (uint8_t *)&device, 1, 0x1000);
	  delay_try(10);
	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

	return device;
}


void wrreg_command(uint8_t reg_addr, uint8_t val_hex){
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
   uint8_t adress = 0x40|reg_addr;
   uint8_t dummy1 = 0x00;
   HAL_SPI_Transmit(&hspi1, (uint8_t*)&adress, 1, 0x1000);
   HAL_SPI_Transmit(&hspi1, (uint8_t*)&dummy1, 1, 0x1000);
   HAL_SPI_Transmit(&hspi1, (uint8_t*)&val_hex, 1, 0x1000);
   delay_us(100);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//void spi_transmit_byte(uint8_t* data, int size){
//	int i=0;
//	for(i=0; i<size; i++){
//		HAL_SPI_Transmit(&hspi1, data[i], 1, HAL_MAX_DELAY);
//		delay_us(2);
//	}
//	return;
//}
//
//uint8_t spi_transmit_receive(uint8_t* address, int size, int outp_size){
//	int i=0;
//	uint8_t data_req[outp_size];
//	for(i=0; i<size; i++){
//		HAL_SPI_Transmit(&hspi1, address[i], 1, HAL_MAX_DELAY);
//		delay_us(2);
//	}
//	HAL_SPI_Receive(&hspi1, data_req, outp_size, HAL_MAX_DELAY);
//	return data_req;
//}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//	uint8_t spi_buf[2];
//	uint8_t* value[27];
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
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  	uint8_t sdatac = 0x11;
	uint8_t buf1 = 0x20;
	uint8_t start = 0x08;
	uint8_t rdatac = 0x10;
	uint8_t rdata = 0x12;
	uint8_t buffer2 = 0x00;
	uint8_t rst = 0x06;

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

  delay_try(100);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
 // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
  delay_try(50);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  delay_try(10);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  delay_try(4);
  HAL_SPI_Transmit(&hspi1, (uint8_t*)&rst, 1, 0x1000);
  delay_try(100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  delay_try(4);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&sdatac, 1, 0x1000);
  delay_try(100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  delay_try(100);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// uint8_t dum = 0x00;


//  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
//
//  delay_try(10);
//  HAL_SPI_Transmit(&hspi1, (uint8_t *)&buffer1, 1, 0x1000);   //MOSI
//  delay_try(2);//delay 4 t_clk
//  HAL_SPI_Transmit(&hspi1, (uint8_t *)&buffer2, 1, 0x1000);  //MOSI
//  delay_try(2);//delay 4 t_clk
//  HAL_SPI_Receive(&hspi1, (uint8_t *)&device, 1, 0x1000);    //MISO
//
//    uint8_t buffer1;
//
//  	buffer1 = buf1 | 0x00;
//    uint8_t read_byte = rreg_command(buffer1, buffer2);
//    uint8_t dev_id[] = "Device Id is : ";
//    HAL_UART_Transmit(&huart2, dev_id, sizeof(dev_id) - 1, 0x1000); // -1 to exclude null terminator
//    char ads[16];
//    sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//    HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//    buffer1 = buf1 | config1;
//    read_byte = rreg_command(buffer1, buffer2);
//    uint8_t c1[] = "Config1 is : ";
//    HAL_UART_Transmit(&huart2, c1, sizeof(c1) - 1, 0x1000); // -1 to exclude null terminator
//    sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//    HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//    buffer1 = buf1 | config2;
//    read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c2[] = "Config2 is : ";
//	HAL_UART_Transmit(&huart2, c2, sizeof(c2) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | config3;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c3[] = "Config3 is : ";
//	HAL_UART_Transmit(&huart2, c3, sizeof(c3) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | loff;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c4[] = "LOFF is : ";
//	HAL_UART_Transmit(&huart2, c4, sizeof(c4) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | ch1set;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c5[] = "Channel 1 Register is : ";
//	HAL_UART_Transmit(&huart2, c5, sizeof(c5) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | bias_sensp;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c6[] = "Bias_sensep is : ";
//	HAL_UART_Transmit(&huart2, c6, sizeof(c6) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | bias_sensn;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c7[] = "Bias_sensen is : ";
//	HAL_UART_Transmit(&huart2, c7, sizeof(c7) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 |loff_sensp;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c8[] = "loff_sensep is : ";
//	HAL_UART_Transmit(&huart2, c8, sizeof(c8) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | loff_sensn;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c9[] = "loff_sensen is : ";
//	HAL_UART_Transmit(&huart2, c9, sizeof(c9) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | loff_flip;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c10[] = "loff_flip is : ";
//	HAL_UART_Transmit(&huart2, c10, sizeof(c10) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | loff_statp;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c11[] = "loff_statp is : ";
//	HAL_UART_Transmit(&huart2, c11, sizeof(c11) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | loff_statn;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c12[] = "loff_statn is : ";
//	HAL_UART_Transmit(&huart2, c12, sizeof(c12) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | gpio;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c13[] = "gpio is : ";
//	HAL_UART_Transmit(&huart2, c13, sizeof(c13) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | misc1;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c14[] = "misc1 is : ";
//	HAL_UART_Transmit(&huart2, c14, sizeof(c14) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | misc2;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c15[] = "misc2 is : ";
//	HAL_UART_Transmit(&huart2, c15, sizeof(c15) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	buffer1 = buf1 | config4;
//	read_byte = rreg_command(buffer1, buffer2);
//	uint8_t c16[] = "config4 is : ";
//	HAL_UART_Transmit(&huart2, c16, sizeof(c16) - 1, 0x1000); // -1 to exclude null terminator
//	sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);

//	HAL_Delay(1000);
  delay_try(100);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	wrreg_command(config1, 0xD5); //D5
	wrreg_command(config2, 0xD1);// D1
	wrreg_command(config3, 0xEF);// E4, E0, E6
	wrreg_command(loff, 0x00);//80
	wrreg_command(ch1set, 0x65);//55
	wrreg_command(ch2set, 0x65);
	wrreg_command(ch3set, 0x65);
	wrreg_command(ch4set, 0x65);
	wrreg_command(ch5set, 0x65);
	wrreg_command(ch6set, 0x65);
	wrreg_command(ch7set, 0x65);
	wrreg_command(ch8set, 0x65);
	wrreg_command(bias_sensp, 0x00);//FF
	wrreg_command(bias_sensn, 0x00);//FF
	wrreg_command(loff_sensp, 0x00);//FF
	wrreg_command(loff_sensn, 0x00);//FF
	wrreg_command(loff_flip, 0x00);
	wrreg_command(gpio, 0x0F); //00, FF, F0
	wrreg_command(misc1, 0x20);
	wrreg_command(config4, 0x02); //00
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  read_byte = rreg_command(buffer1, buffer2);

//		buffer1 = buf1 | 0x00;
//	    read_byte = rreg_command(buffer1, buffer2);
////	    uint8_t dev_id[] = "Device Id is : ";
//	    HAL_UART_Transmit(&huart2, dev_id, sizeof(dev_id) - 1, 0x1000); // -1 to exclude null terminator
////	     char ads[16];
//	    sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	    HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	    buffer1 = buf1 | config1;
//	    read_byte = rreg_command(buffer1, buffer2);
////	    uint8_t c1[] = "Config1 is : ";
//	    HAL_UART_Transmit(&huart2, c1, sizeof(c1) - 1, 0x1000); // -1 to exclude null terminator
//	    sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//	    HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//	    buffer1 = buf1 | config2;
//	    read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c2[] = "Config2 is : ";
//		HAL_UART_Transmit(&huart2, c2, sizeof(c2) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | config3;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c3[] = "Config3 is : ";
//		HAL_UART_Transmit(&huart2, c3, sizeof(c3) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | loff;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c4[] = "LOFF is : ";
//		HAL_UART_Transmit(&huart2, c4, sizeof(c4) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | ch1set;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c5[] = "Channel 1 Register is : ";
//		HAL_UART_Transmit(&huart2, c5, sizeof(c5) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | bias_sensp;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c6[] = "Bias_sensep is : ";
//		HAL_UART_Transmit(&huart2, c6, sizeof(c6) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | bias_sensn;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c7[] = "Bias_sensen is : ";
//		HAL_UART_Transmit(&huart2, c7, sizeof(c7) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 |loff_sensp;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c8[] = "loff_sensep is : ";
//		HAL_UART_Transmit(&huart2, c8, sizeof(c8) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | loff_sensn;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c9[] = "loff_sensen is : ";
//		HAL_UART_Transmit(&huart2, c9, sizeof(c9) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | loff_flip;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c10[] = "loff_flip is : ";
//		HAL_UART_Transmit(&huart2, c10, sizeof(c10) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | loff_statp;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c11[] = "loff_statp is : ";
//		HAL_UART_Transmit(&huart2, c11, sizeof(c11) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | loff_statn;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c12[] = "loff_statn is : ";
//		HAL_UART_Transmit(&huart2, c12, sizeof(c12) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | gpio;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c13[] = "gpio is : ";
//		HAL_UART_Transmit(&huart2, c13, sizeof(c13) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | misc1;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c14[] = "misc1 is : ";
//		HAL_UART_Transmit(&huart2, c14, sizeof(c14) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | misc2;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c15[] = "misc2 is : ";
//		HAL_UART_Transmit(&huart2, c15, sizeof(c15) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//
//		buffer1 = buf1 | config4;
//		read_byte = rreg_command(buffer1, buffer2);
////		uint8_t c16[] = "config4 is : ";
//		HAL_UART_Transmit(&huart2, c16, sizeof(c16) - 1, 0x1000); // -1 to exclude null terminator
//		sprintf(ads, "%u\n", read_byte); // Dereference the pointer to get the value
//		HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);


  delay_try(10);

//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
  delay_try(4);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  delay_try(4);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
//  HAL_SPI_Transmit(&hspi1, (uint8_t *)&start, 1, 0x1000);
  delay_try(100);

  //send RDATAC command
//  delay_us(4);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&rdatac, 1, 0x1000);
  delay_try(4);
  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

//  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
//  uint32_t read_dat;
//  while(1){

  uint8_t suc[] = "success";
  	uint32_t status_reg;
	uint8_t inbyte;
	status_reg = 0x00000000;
	uint32_t output[8];
	uint32_t dataPacket = 0x00000000;
	uint8_t dataByte;

  while(1){
	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == 0){
		  HAL_UART_Transmit(&huart2, suc, sizeof(suc) - 1, 0x1000);
		  for (int j = 0; j < 3; j++) {
			HAL_SPI_Receive(&hspi1, (uint8_t*)&inbyte, 1, 0x1000); // Pass the address of inbyte
			status_reg = (status_reg << 8) | inbyte;
		  }

		  for(int i = 0; i < 8; i++) {
			for(int j = 0; j < 3; j++) {
				HAL_SPI_Receive(&hspi1, (uint8_t*)&dataByte, 1, 0x1000); // Corrected variable name
				dataPacket = (dataPacket << 8) | dataByte;
			}
			output[i] = dataPacket;
			dataPacket = 0;
		}
		delay_try(1000);


		  			char ads_data[16];
		  			sprintf(ads_data, "%lu\r\n", status_reg);
//		  			printf("Status_reg: %s", ads_data);
		  			HAL_UART_Transmit(&huart2, (uint8_t*)ads_data, strlen(ads_data), 0x1000);
		  			for(int i = 0; i < 8; i++) {
		  				 sprintf(ads_data, "%lu\r\n", output[i]);
//		  				 printf("Channel : %s", ads_data);
		  				 HAL_UART_Transmit(&huart2, (uint8_t*)ads_data, strlen(ads_data), 0x1000);

		  			}
		  			delay_try(1000);
		 HAL_Delay(100);
		  		}
	  else{
		  continue;
	  }
  }
	  //  HAL_UART_Transmit(&huart2, channel, sizeof(channel) - 1, 0x1000); // -1 to exclude null terminator
////	    char ads_r[32*8];
////	    sprintf(ads_r, "%u\n", read_dat); // Dereference the pointer to get the value
////	    HAL_UART_Transmit(&huart2, (uint8_t *)&ads, strlen(ads), 0x1000);
//  }
  delay_try(4);



//    HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_RESET);
//
//  HAL_GPIO_WritePin(GPIOC, RESET_Pin, GPIO_PIN_RESET);
//  delay_us(10);
//  HAL_GPIO_WritePin(GPIOC, RESET_Pin, GPIO  _PIN_SET);
//
//  delay_us(10);

//  spi_buf[0] = 0x11; //stop cont data read
//  spi_transmit_byte(*spi_buf, 1);
//  //delay
//
//  spi_buf[0] = 0x0A; //stop complete conversion
//  spi_transmit_byte(*spi_buf, 1);

  //spi_send(spi_buf[0])
  //delay

//  spi_buf[0] = 0x20; //device id
//  spi_buf[1] = 0x00;
//  *value[0] = spi_transmit_receive(*spi_buf, 2, 1); ///
//
//  HAL_UART_Transmit(&huart2, &value[0], 1, HAL_MAX_DELAY);
/*
  spi_buf[0] = 0x41; //config1
  spi_buf[1] = 0x00;
  spi_buf[2] = 0xC6;
  spi_transmit_byte(spi_buf, 3);//delay
  //spi_send
  //delay

  spi_buf[0] = 0x22; //config2, same as reset
  spi_buf[1] = 0x00;
  *value[0] = spi_transmit_receive(spi_buf, 2, 1);
   ///

  HAL_UART_Transmit(&huart2, value[0], 1, HAL_MAX_DELAY);


  //value = spi_sendreceive
  //delay

  spi_buf[0] = 0x43; //config3, same as reset
  spi_buf[1] = 0x00;
  spi_buf[2] = 0xE0;
  spi_transmit_byte(spi_buf, 3); ///

  HAL_UART_Transmit(&huart2, value[0], 1, HAL_MAX_DELAY);


  //value = spi_sendreceive
  //delay

  spi_buf[0] = 0x24; //config4, same as reset
  spi_buf[1] = 0x00;
  *value[0] = spi_transmit_receive(spi_buf, 2, 1); ///

  HAL_UART_Transmit(&huart2, value[0], 1, HAL_MAX_DELAY);

  spi_buf[0] = 0x45; //channels
  spi_buf[1] = 0x07;
  spi_buf[2] = 0x00;
  spi_transmit_byte(spi_buf, 3);

  spi_buf[0] = 0x2D; //bias - sense p, same as reset
  spi_buf[1] = 0x00;
  *value[0] = spi_transmit_receive(spi_buf, 2, 1); ///

  HAL_UART_Transmit(&huart2, value[0], 1, HAL_MAX_DELAY);


  spi_buf[0] = 0x4E; //bias, sense n, all enabled
  spi_buf[1] = 0x00;
  spi_buf[2] = 0xFF;
  spi_transmit_byte(spi_buf, 3);
  //spi_send
    //delay

  spi_buf[0] = 0x4F; //loff-sense p, all enabled
  spi_buf[1] = 0x00;
  spi_buf[2] = 0xFF;
  spi_transmit_byte(spi_buf, 3);
  //spi_send
  //delay

  spi_buf[0] = 0x30; //loff - sense n, same as reset
  spi_buf[1] = 0x00;
  *value[0] = spi_transmit_receive(spi_buf, 2, 1);

  HAL_UART_Transmit(&huart2, &value[0], 1, HAL_MAX_DELAY);

    //value = spi_sendreceive
    //delay

  spi_buf[0] = 0x31; //loff_flip, same as reset
  spi_buf[1] = 0x00;
  *value[0] = spi_transmit_receive(spi_buf, 2, 1);

  HAL_UART_Transmit(&huart2, (uint8_t *) value[0], 1, HAL_MAX_DELAY);

    //value = spi_sendreceive
    //delay

  spi_buf[0] = 0x32; //loff stat p, same as reset
  spi_buf[1] = 0x00;
  *value[0] = spi_transmit_receive(spi_buf, 2, 1);

  HAL_UART_Transmit(&huart2, (uint8_t *) value[0], 1, HAL_MAX_DELAY);

  spi_buf[0] = 0x33; //bias - sense p, same as reset
  spi_buf[1] = 0x00;
  *value[0] = spi_transmit_receive(spi_buf, 2, 1);

    HAL_UART_Transmit(&huart2, (uint8_t *) value[0], 1, HAL_MAX_DELAY);

  spi_buf[0] = 0x34; //gpio, same as reset
  spi_buf[1] = 0x00;
  *value[0] = spi_transmit_receive(spi_buf, 2, 1);

    HAL_UART_Transmit(&huart2, (uint8_t *) value[0], 1, HAL_MAX_DELAY);

  spi_buf[0] = 0x55; //misc1
  spi_buf[1] = 0x00;
  spi_buf[2] = 0x20;
  spi_transmit_byte(spi_buf, 3);
    //spi_send
    //delay

  spi_buf[0] = 0x36; //misc2
  spi_buf[1] = 0x00;
  *value[0] = spi_transmit_receive(spi_buf, 2, 1);

    HAL_UART_Transmit(&huart2, (uint8_t *) value[0], 1, HAL_MAX_DELAY);

  spi_buf[0] = 0x57; //config4
  spi_buf[1] = 0x00;
  spi_buf[2] = 0x02;
  spi_transmit_byte(spi_buf, 3);
  //spi_send
      //delay

  spi_buf[0] = 0x08; //start conversions
  spi_transmit_byte(spi_buf, 1);
  //spi_send

  spi_buf[0] = 0x10; //read data cont
  spi_transmit_byte(spi_buf, 3);
  //value = spi_sendreceive
*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

 // uint8_t spi_buffer[27];

 // while (1){
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//		HAL_SPI_Receive(&hspi1, spi_buffer, 27, HAL_MAX_DELAY);
	    /* USER CODE END WHILE */
//		int i=0;
//		for(i=0;i<27;i++){
//				HAL_UART_Transmit(&huart2, spi_buffer[i], 1, HAL_MAX_DELAY);
//				HAL_UART_Transmit(&huart2," ", 1, HAL_MAX_DELAY);
//			}
//			HAL_UART_Transmit(&huart2,"\r\n", 1, HAL_MAX_DELAY);
//  }
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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  huart2.Init.BaudRate = 115200;
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

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

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

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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
