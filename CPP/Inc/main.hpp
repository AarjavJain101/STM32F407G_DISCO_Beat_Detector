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
#include "DETECTOR.hpp"
#include <vector>

/* ========== CONSTANTS ========== */

/* AUDIO Constants*/
#define AUDIO_BUFFER_SIZE           8192
#define FFT_SIZE                    AUDIO_BUFFER_SIZE / 2
#define START_OF_FIRST_HALF         0
#define START_OF_SECOND_HALF        FFT_SIZE
#define NUM_FREQ_BINS               FFT_SIZE / 2
#define SAMPLE_RATE                 93750


/*  ============== LED AND PIXEL STRIP CONSTANTS ============== */
#define SET_COLOR(r, g, b)      ((g << 16U) | (r << 8U) | b)              // ORDER IS G-R-B because of WS2812B

#define NUM_PIXELS              10
#define LEDS_PER_PIXEL          3               // RGB only in WS2812B

#define GREEN_IDX               0
#define RED_IDX                 1
#define BLUE_IDX                2


/* ========== EXTERNS ========== */
extern bool is_data_sent;
extern int16_t raw_audio_buffer[AUDIO_BUFFER_SIZE];
extern volatile int16_t sample_i2s;
extern bool isI2SBufferHalfDone;
extern bool isI2SBufferDone;





#endif /* INC_MAIN_HPP_ */
