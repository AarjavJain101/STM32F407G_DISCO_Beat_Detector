/**
 * @file DETECTOR.hpp
 * @brief This file contains the function prototypes for the detector module
 * @author Aarjav Jain
 * @date 2024-08-01
 */

#ifndef _DETECTOR_HPP_
#define _DETECTOR_HPP_

/* ========== INCLUDES ========== */
#include <vector>
#include "arm_math.h"
#include "main.hpp"


 
/* ========== CONSTANTS ========== */
#define DO_INVERSE_FFT              1
#define DO_FFT                      0



/* ========== FUNCTION PROTOTYPES ========== */
void DETECTOR_fft_init();
void DETECTOR_perform_detection(int16_t* raw_audio_buffer);

#endif /* _DETECTOR_HPP_ */
