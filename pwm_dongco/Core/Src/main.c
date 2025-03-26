#include "main.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

TIM_HandleTypeDef* bo_dinh_thoi_dong_co; // Con trỏ tới bộ định thời PWM
uint16_t gia_tri_pwm = 300; // Giá trị PWM ban đầu
uint8_t trang_thai_dong_co = 0; // Trạng thái động cơ (0: tắt, 1: bật)
uint32_t time_chong_nhay = 0; // Biến lưu thời gian debounce

const uint16_t pwm_toi_da = 999; // Giá trị PWM tối đa
const uint16_t pwm_toi_thieu = 100; // Giá trị PWM tối thiểu
const uint16_t buoc_pwm = 50; // Bước tăng/giảm PWM
const uint32_t time_delay_50 = 50; // Khoảng thời gian debounce (50ms)

// Khởi tạo động cơ với bộ định thời PWM
void Khoi_Tao_Dong_Co(TIM_HandleTypeDef* htim1) {
    bo_dinh_thoi_dong_co = htim1; // Gán con trỏ bộ định thời cho biến bo_dinh_thoi_dong_co
}

// Hàm khởi động PWM
void Bat_Dau_Dong_Co() {
    HAL_TIM_PWM_Start(bo_dinh_thoi_dong_co, TIM_CHANNEL_1); // Bắt đầu PWM trên kênh 1 của bộ định thời
}

// Hàm kiểm tra trạng thái các nút nhấn
void Kiem_Tra_Nut_Nhan() {
	static uint8_t trang_thai_nut_truoc = GPIO_PIN_SET; // Trạng thái trước của nút

	    uint32_t thoi_gian_hien_tai = HAL_GetTick();
	    if (thoi_gian_hien_tai - time_chong_nhay < time_delay_50) return;

	    uint8_t trang_thai_nut_hien_tai = HAL_GPIO_ReadPin(button_GPIO_Port, button_Pin);

	    // Kiểm tra nếu nút nhấn được nhả ra sau khi nhấn (edge detection)
	    if (trang_thai_nut_truoc == GPIO_PIN_RESET && trang_thai_nut_hien_tai == GPIO_PIN_SET) {
	        trang_thai_dong_co = !trang_thai_dong_co; // Đảo trạng thái động cơ
	        time_chong_nhay = thoi_gian_hien_tai; // Cập nhật thời gian debounce
	    }

	    trang_thai_nut_truoc = trang_thai_nut_hien_tai; // Cập nhật trạng thái nút

    // Nút tăng tốc độ
    if (HAL_GPIO_ReadPin(tang_GPIO_Port, tang_Pin) == GPIO_PIN_RESET && gia_tri_pwm + buoc_pwm <= pwm_toi_da) {
        gia_tri_pwm += buoc_pwm;
        time_chong_nhay = thoi_gian_hien_tai;
    }

    // Nút giảm tốc độ
    if (HAL_GPIO_ReadPin(giam_GPIO_Port, giam_Pin) == GPIO_PIN_RESET && gia_tri_pwm - buoc_pwm >= pwm_toi_thieu) {
        gia_tri_pwm -= buoc_pwm;
        time_chong_nhay = thoi_gian_hien_tai;
    }
}


// Hàm cập nhật giá trị PWM cho động cơ
void Cap_Nhat_PWM() {
	__HAL_TIM_SET_COMPARE(bo_dinh_thoi_dong_co, TIM_CHANNEL_1, trang_thai_dong_co ? gia_tri_pwm : 0);
}

TIM_HandleTypeDef htim1; // Khai báo biến điều khiển Timer 1
int main(void)
{
	 HAL_Init(); // Khởi tạo thư viện HAL
	    SystemClock_Config(); // Cấu hình xung hệ thống
	    MX_GPIO_Init(); // Khởi tạo GPIO
	    MX_TIM1_Init(); // Khởi tạo Timer 1

	    Khoi_Tao_Dong_Co(&htim1); // Khởi tạo động cơ với Timer 1
	    Bat_Dau_Dong_Co(); // Bắt đầu PWM cho động cơ

	    while (1) {
	        Kiem_Tra_Nut_Nhan(); // Kiểm tra các nút nhấn
	        Cap_Nhat_PWM(); // Cập nhật giá trị PWM cho động cơ
	        HAL_Delay(10); // Delay 10ms để tránh lặp quá nhanh
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

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : led1_Pin */
  GPIO_InitStruct.Pin = led1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(led1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : giam_Pin button_Pin */
  GPIO_InitStruct.Pin = giam_Pin|button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : tang_Pin */
  GPIO_InitStruct.Pin = tang_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(tang_GPIO_Port, &GPIO_InitStruct);

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
