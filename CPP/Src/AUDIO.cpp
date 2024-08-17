/**
 * @file AUDIO.cpp
 * @brief This file contains the implementation for the audio module
 * @author Aarjav Jain
 * @date 2024-08-01
 */

#include "AUDIO.hpp"

static uint32_t time1 = 0;
static uint32_t time2 = 0;
static float g = 0;


/**
 * @brief I2S Half complete callback
 * @param hi2s I2S handle
 * @retval None
 */ 
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  time1 = HAL_GetTick();
  isI2SBufferHalfDone = true;
}

/**
 * @brief I2S complete callback
 * @param hi2s I2S handle
 * @retval None
 */
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  time2 = HAL_GetTick();
  isI2SBufferDone = true;
  sample_i2s = raw_audio_buffer[0];
  g = (float)(time2 - time1);
}
