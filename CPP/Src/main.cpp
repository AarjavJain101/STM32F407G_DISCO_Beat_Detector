/*
 * main.cpp
 *
 *  Created on: Aug 1, 2024
 *      Author: aarjav
 */

#include "main.hpp"


void CppMain()
{
	// Enable sound reading using DMA
	HAL_I2S_Receive_DMA(&hi2s3, (uint16_t*) data_i2s, sizeof(data_i2s) / sizeof(data_i2s[0]));

	// LED flashing
	uint8_t red_val = 0;
	while (1) {
      uint16_t* pwm_data = PIXELS_color_pixels(0, 10, red_val, 0, 0);

      HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwm_data, BITS_PER_PIXEL * NUM_PIXELS + BITS_FOR_RESET);
      while (!is_data_sent){};
      is_data_sent = false;
      red_val = ((red_val + 1) % 2) * 255;
      HAL_Delay(500);
	}
}

