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


void CppMain()
{
	// Enable sound reading using DMA
	HAL_I2S_Receive_DMA(&hi2s3, (uint16_t*) raw_audio_buffer, sizeof(raw_audio_buffer) / sizeof(raw_audio_buffer[0]));

      // Initiliaze FFT and variables for detection
      DETECTOR_init();



	// LED flashing
      uint16_t* pwm_data = PIXELS_color_pixels(0, 10, 255, 255, 255);
	while (1) {
            
            // if (isI2SBufferHalfDone)                                  // Audio buffer is 4096 but I do half at a time to avoid overwriting
            // {
            //       DETECTOR_perform_detection(&raw_audio_buffer[START_OF_FIRST_HALF]);  
            //       pwm_data = PIXELS_sound_react();
            //       isI2SBufferHalfDone = false;                                            // Reset flag
            // } 
            // else if (isI2SBufferDone) 
            // {
            //       DETECTOR_perform_detection(&raw_audio_buffer[START_OF_SECOND_HALF]);
            //       pwm_data = PIXELS_sound_react();
            //       isI2SBufferDone = false;                                                // Reset flag
            // }

            // HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwm_data, BITS_PER_PIXEL * NUM_PIXELS + BITS_FOR_RESET);
            // while (!is_data_sent){};
            // is_data_sent = false;

            // UART transmit hello world
            char hello[] = "Hello World!\r\n";
            HAL_UART_Transmit_DMA(&huart2, (uint8_t*)hello, sizeof(hello));
            HAL_Delay(300);
	}
}


// Register the DMA UART cplt callback to printf (done)


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
