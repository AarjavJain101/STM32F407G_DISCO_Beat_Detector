/**
 * @file AUDIO.hpp
 * @brief This file contains the function prototypes for the audio module
 * @author Aarjav Jain
 * @date 2024-08-01
 */

#ifndef _AUDIO_HPP_
#define _AUDIO_HPP_

#include "main.hpp"

/* ========= FUNCTION DECLARATIONS ========== */

/* CALLBACKS */
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s);

/* PERSONAL FUNCTIONS */


#endif /* _AUDIO_HPP_ */