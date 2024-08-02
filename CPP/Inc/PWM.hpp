/**
 * @file PWM.hpp
 * @brief header file for PWM.cpp
 * @author Aarjav Jain
 * @date 2024-07-29
 */


/*  ============================ PWM ABSTRACTION ============================ 
 *  
 *  1.25us per bit = 800kHz. For STM32F407G-DISC1, 168MHz clock. Prescaler = 1, ARR = 210
 *  Data sheet of WS2812B results in T1H = 134, T0H = 67
 * 
 * uint16_t is used because the PWM data is based on counter period which is 16 bits.
 */


#ifndef _PWM_HPP_
#define _PWM_HPP_

#include "CONSTANTS.hpp"
#include <stdint.h>
#include <stdbool.h>



/* =================== TIMING/WIDTH CONSTANTS =================== */

#define PWM_PERIOD_COUNT              210
#define PWM_T1H_WS2812B               134
#define PWM_T0H_WS2812B               67
#define PWM_RESET_WS2812B             0

#define BITS_PER_LED                  8
#define BITS_PER_PIXEL                BITS_PER_LED * LEDS_PER_PIXEL
#define BITS_FOR_RESET                50            // requires > 50us of low signal to reset. 50 * 1.25 = 62.5us > 50us 


/* =================== PUBLIC FUNCTIONS DECLARATIONS =================== */
uint16_t* PWM_set_pwm_all_pixels(uint8_t pixel_strip[NUM_PIXELS][LEDS_PER_PIXEL]);
void _set_reset_bits_pwm();
void PWM_set_pwm_one_pixel(uint16_t pixel_idx, uint32_t rgb_val, uint8_t num_bits);
void _set_value_as_pwm(uint32_t pwm_data_idx, uint32_t value, uint8_t num_bits);


#endif /* _PWM_HPP_ */
