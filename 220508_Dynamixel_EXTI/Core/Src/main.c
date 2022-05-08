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
 UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2,(uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size)
{
    unsigned short i, j;
    unsigned short crc_table[256] = {
        0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
        0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
        0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
        0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
        0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
        0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
        0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
        0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
        0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
        0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
        0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
        0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
        0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
        0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
        0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
        0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
        0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
        0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
        0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
        0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
        0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
        0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
        0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
        0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
        0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
        0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
        0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
        0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
        0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
        0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
        0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
        0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
    };

    for(j = 0; j < data_blk_size; j++)
    {
        i = ((unsigned short)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
        crc_accum = (crc_accum << 8) ^ crc_table[i];
    }

    return crc_accum;
}
int dir = 0;
int compare = 1;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	printf("EXTI!!\r\n");
	if(GPIO_Pin == GPIO_PIN_8){
		if(dir == 0) dir = 1;
		else dir = 0;
		compare = 0;
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  uint8_t CRC_H;
  uint8_t CRC_L;

  unsigned char TxPacket_Oper_Speedmode[]= 		{0xFF,0xFF,0xFD,0x00,0x01,0x06,0x00,0x03,0x0B,0x00,0x01,0x00,0x00}; // Operating mode length : 13
  unsigned char TxPacket_Torque_On[]= 	   		{0xFF,0xFF,0xFD,0x00,0x01,0x06,0x00,0x03,0x40,0x00,0x01,0x00,0x00}; // Torque on length : 13
  unsigned char TxPacket_Torque_Off[]= 	   		{0xFF,0xFF,0xFD,0x00,0x01,0x06,0x00,0x03,0x40,0x00,0x00,0x00,0x00}; // Torque on length : 13

  unsigned char TxPacket_Goal_Velocity[]= 	 	{0xFF,0xFF,0xFD,0x00,0x01,0x09,0x00,0x03,0x68,0x00,0x32,0x00,0x00,0x00,0x00,0x00}; // Goal Velocity length : 16
  unsigned char TxPacket_Change_Direction_Reverse[]= 	{0xFF,0xFF,0xFD,0x00,0x01,0x06,0x00,0x03,0x0A,0x00,0x01,0x00,0x00}; // Change Direction_Reverse length : 13
  unsigned char TxPacket_Change_Direction_Normal[]= 	{0xFF,0xFF,0xFD,0x00,0x01,0x06,0x00,0x03,0x0A,0x00,0x00,0x00,0x00}; // Change Direction_Reverse length : 13

  uint16_t CRC_total = update_crc(0x0, TxPacket_Oper_Speedmode , 11);
  CRC_H = (CRC_total>>8) & 0x00FF;
  CRC_L = (CRC_total & 0x00FF);
  TxPacket_Oper_Speedmode[11] = CRC_L;
  TxPacket_Oper_Speedmode[12] = CRC_H;

  CRC_total = update_crc(0x0, TxPacket_Torque_On , 11);
  CRC_H = (CRC_total>>8) & 0x00FF;
  CRC_L = (CRC_total & 0x00FF);
  TxPacket_Torque_On[11] = CRC_L;
  TxPacket_Torque_On[12] = CRC_H;

  CRC_total = update_crc(0x0, TxPacket_Torque_Off , 11);
  CRC_H = (CRC_total>>8) & 0x00FF;
  CRC_L = (CRC_total & 0x00FF);
  TxPacket_Torque_Off[11] = CRC_L;
  TxPacket_Torque_Off[12] = CRC_H;

  CRC_total = update_crc(0x0, TxPacket_Goal_Velocity , 14);
  CRC_H = (CRC_total>>8) & 0x00FF;
  CRC_L = (CRC_total & 0x00FF);
  TxPacket_Goal_Velocity[14] = CRC_L;
  TxPacket_Goal_Velocity[15] = CRC_H;

  CRC_total = update_crc(0x0, TxPacket_Change_Direction_Reverse , 11);
  CRC_H = (CRC_total>>8) & 0x00FF;
  CRC_L = (CRC_total & 0x00FF);
  TxPacket_Change_Direction_Reverse[11] = CRC_L;
  TxPacket_Change_Direction_Reverse[12] = CRC_H;

  CRC_total = update_crc(0x0, TxPacket_Change_Direction_Normal , 11);
  CRC_H = (CRC_total>>8) & 0x00FF;
  CRC_L = (CRC_total & 0x00FF);
  TxPacket_Change_Direction_Normal[11] = CRC_L;
  TxPacket_Change_Direction_Normal[12] = CRC_H;

  uint8_t rx_data[16]={0};

	  HAL_UART_Transmit(&huart1, TxPacket_Oper_Speedmode, sizeof(TxPacket_Oper_Speedmode)/sizeof(unsigned char), 0xFF);
	  HAL_UART_Receive(&huart1, rx_data, 16, 10);
	  for(int idx = 0; idx <16 ; idx ++)
	 	  printf("0x%x ",rx_data[idx]);
	 	  printf("\r\n");

	  HAL_UART_Transmit(&huart1, TxPacket_Torque_On, sizeof(TxPacket_Torque_On)/sizeof(unsigned char), 0xFF);
	  HAL_UART_Receive(&huart1, rx_data, 16, 10);
	  for(int idx = 0; idx <16 ; idx ++)
	 	  printf("0x%x ",rx_data[idx]);
	 	  printf("\r\n");

	  HAL_UART_Transmit(&huart1, TxPacket_Goal_Velocity, sizeof(TxPacket_Goal_Velocity)/sizeof(unsigned char), 0xFF);
	  HAL_UART_Receive(&huart1, rx_data, 16, 10);
	  for(int idx = 0; idx <16 ; idx ++)
	  printf("0x%x ",rx_data[idx]);
	  printf("\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  if(dir==1 && compare == 0){
		  HAL_UART_Transmit(&huart1, TxPacket_Torque_Off, sizeof(TxPacket_Torque_Off)/sizeof(unsigned char), 0xFF);
		  HAL_UART_Receive(&huart1, rx_data, 16, 10);
		  for(int idx = 0; idx <16 ; idx ++)
		 	  printf("0x%x ",rx_data[idx]);
		 	  printf("\r\n");

		  HAL_UART_Transmit(&huart1, TxPacket_Change_Direction_Reverse, sizeof(TxPacket_Change_Direction_Reverse)/sizeof(unsigned char), 0xFF);
		  HAL_UART_Receive(&huart1, rx_data, 16, 10);
		  for(int idx = 0; idx <16 ; idx ++)
		 	  printf("0x%x ",rx_data[idx]);
		 	  printf("\r\n");

		  HAL_UART_Transmit(&huart1, TxPacket_Torque_On, sizeof(TxPacket_Torque_On)/sizeof(unsigned char), 0xFF);
		  HAL_UART_Receive(&huart1, rx_data, 16, 10);
		  for(int idx = 0; idx <16 ; idx ++)
			  printf("0x%x ",rx_data[idx]);
			  printf("\r\n");

			  HAL_UART_Transmit(&huart1, TxPacket_Goal_Velocity, sizeof(TxPacket_Goal_Velocity)/sizeof(unsigned char), 0xFF);
			  HAL_UART_Receive(&huart1, rx_data, 16, 10);
			  for(int idx = 0; idx <16 ; idx ++)
			  printf("0x%x ",rx_data[idx]);
			  printf("\r\n");
		 compare = 1;
	  }

	  else if(dir == 0 && compare ==0){
		  HAL_UART_Transmit(&huart1, TxPacket_Torque_Off, sizeof(TxPacket_Torque_Off)/sizeof(unsigned char), 0xFF);
		  HAL_UART_Receive(&huart1, rx_data, 16, 10);
		  for(int idx = 0; idx <16 ; idx ++)
		 	  printf("0x%x ",rx_data[idx]);
		 	  printf("\r\n");

		  HAL_UART_Transmit(&huart1, TxPacket_Change_Direction_Normal, sizeof(TxPacket_Change_Direction_Normal)/sizeof(unsigned char), 0xFF);
		  HAL_UART_Receive(&huart1, rx_data, 16, 10);
		  for(int idx = 0; idx <16 ; idx ++)
		 	  printf("0x%x ",rx_data[idx]);
		 	  printf("\r\n");

		  HAL_UART_Transmit(&huart1, TxPacket_Torque_On, sizeof(TxPacket_Torque_On)/sizeof(unsigned char), 0xFF);
		  HAL_UART_Receive(&huart1, rx_data, 16, 10);
		  for(int idx = 0; idx <16 ; idx ++)
			  printf("0x%x ",rx_data[idx]);
			  printf("\r\n");

			  HAL_UART_Transmit(&huart1, TxPacket_Goal_Velocity, sizeof(TxPacket_Goal_Velocity)/sizeof(unsigned char), 0xFF);
			  HAL_UART_Receive(&huart1, rx_data, 16, 10);
			  for(int idx = 0; idx <16 ; idx ++)
			  printf("0x%x ",rx_data[idx]);
			  printf("\r\n");

		 compare = 1;
	  }
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
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 57600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  huart2.Init.BaudRate = 57600;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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
