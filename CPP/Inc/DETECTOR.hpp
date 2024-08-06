/**
 * @file DETECTOR.hpp
 * @brief This file contains the function prototypes for the detector module
 * @author Aarjav Jain
 * @date 2024-08-01
 */

#ifndef _DETECTOR_HPP_
#define _DETECTOR_HPP_

/* ========== INCLUDES ========== */
#include "CONSTANTS.hpp"


/* ========== STRUCTS ========== */
typedef struct {
    bool do_bass;
    bool do_clap;
    bool do_hihat;
} beat_actions_t;

 
/* ========== FUNCTION PROTOTYPES ========== */ 
void DETECTOR_init();
void DETECTOR_perform_detection(int16_t* raw_audio_buffer);


/* ========== EXTERNS ========== */
extern beat_actions_t beat_actions;


#endif /* _DETECTOR_HPP_ */
