/**
 * @file AUDIO.cpp
 * @brief This file contains the implementation for the audio module
 * @author Aarjav Jain
 * @date 2024-08-01
 */

#include "AUDIO.hpp"


/**
 * @brief I2S Half complete callback
 * @param hi2s I2S handle
 * @retval None
 */
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  isI2SBufferHalfDone = true;
}

/**
 * @brief I2S complete callback
 * @param hi2s I2S handle
 * @retval None
 */
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  isI2SBufferDone = true;
  sample_i2s = raw_audio_buffer[0];
}
