/*
 * main.hpp
 *
 *  Created on: Aug 1, 2024
 *      Author: aarjav
 */

#ifndef INC_MAIN_HPP_
#define INC_MAIN_HPP_


/* ========== INCLUDES ========== */

/* HAL INCLUDES */
#include "main.h"

/* MY INCLUDES */
#include "CONSTANTS.hpp"
#include "DETECTOR.hpp"
#include "PIXELS.hpp"
#include "PWM.hpp"
#include <vector>

/* ========== EXTERNS ========== */
extern bool is_data_sent;
extern int16_t raw_audio_buffer[AUDIO_BUFFER_SIZE];
extern volatile int16_t sample_i2s;
extern volatile bool isI2SBufferHalfDone;
extern volatile bool isI2SBufferDone;





#endif /* INC_MAIN_HPP_ */
