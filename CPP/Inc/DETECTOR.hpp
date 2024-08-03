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
#define CHANNELS                    1
#define HISTORY_SECONDS             1
#define CLAP_RANGE_LOW              11
#define HIHAT_RANGE_LOW             27

#define TOTAL_SUB_BANDS             39 // Each sub band is a range of 5 * frequency resolution. it is ~230Hz wide and there are 39 of these

#define REAL                        0
#define IMAG                        1

#define HIHAT_GAP_ARRAY_SIZE        35




/* ========== FUNCTION PROTOTYPES ========== */
void DETECTOR_init();
void DETECTOR_perform_detection(int16_t* raw_audio_buffer);

#endif /* _DETECTOR_HPP_ */
