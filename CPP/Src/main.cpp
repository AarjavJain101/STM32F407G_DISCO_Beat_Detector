/*
 * main.cpp
 *
 *  Created on: Aug 1, 2024
 *      Author: aarjav
 */

#include "main.hpp"
#include "stdio.h"

using namespace std;

bool is_data_sent = false;
int16_t raw_audio_buffer[AUDIO_BUFFER_SIZE];
volatile int16_t sample_i2s = 0;
volatile bool isI2SBufferHalfDone = false;
volatile bool isI2SBufferDone = false;
bool UART_sent = false;


void CppMain()
{
	// Enable sound reading using DMA
	HAL_I2S_Receive_DMA(&hi2s3, (uint16_t*) raw_audio_buffer, sizeof(raw_audio_buffer) / sizeof(raw_audio_buffer[0]));

  // Initiliaze FFT and variables for detection
  DETECTOR_init();


  uint16_t* pwm_data = PIXELS_color_pixels(0, 10, 255, 255, 255);
	while (1) {   
    if (isI2SBufferHalfDone)                                  // Audio buffer is 4096 but I do half at a time to avoid overwriting
    {
          uint32_t start = HAL_GetTick();
          HAL_UART_Transmit_DMA(&huart2, (uint8_t*)(&raw_audio_buffer[START_OF_FIRST_HALF]), sizeof(raw_audio_buffer) / 2); 
          while (!UART_sent){};
          UART_sent = false;
          uint32_t end = HAL_GetTick();
          uint32_t time = end - start;

          // DETECTOR_perform_detection(&raw_audio_buffer[START_OF_FIRST_H  ALF]);  
          // pwm_data = PIXELS_sound_react();
          // isI2SBufferHalfDone = false;                                            // Reset flag
    } 
    else if (isI2SBufferDone) 
    {
          uint32_t start = HAL_GetTick();
          HAL_UART_Transmit_DMA(&huart2, (uint8_t*)(&raw_audio_buffer[START_OF_SECOND_HALF]), sizeof(raw_audio_buffer) / 2); 
          while (!UART_sent){};
          UART_sent = false;
          uint32_t end = HAL_GetTick();

          // DETECTOR_perform_detection(&raw_audio_buffer[START_OF_SECOND_HALF]);
          // pwm_data = PIXELS_sound_react();
          // isI2SBufferDone = false;                                                // Reset flag
    }

    // HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwm_data, BITS_PER_PIXEL * NUM_PIXELS + BITS_FOR_RESET);
    // while (!is_data_sent){};
    // is_data_sent = false;
	}
}


// UART done callback
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  UART_sent = true;
}


/**
 * @brief Overriden write function to use SWV
 * @param file: The file to write to
 * @param ptr: The pointer to the data
 * @param len: The length of the data
 * @return The length of the data
 */
int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    ITM_SendChar(*ptr++);
  }
  return len;
}
