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

#include "main.hpp"

/* ============================ PROTOTYPES ============================ */
static struct {
    uint8_t red = 0, green = 0, blue = 0;
    float decayRate = 0.0;
    uint8_t redrawHihatCounter = 0; 
} state;


/* =================== PUBLIC FUNCTIONS DECLARATIONS =================== */

/* PERSONAL FUNCTIONS */
uint16_t* PWM_set_pwm_all_pixels(uint8_t pixel_strip[NUM_PIXELS][LEDS_PER_PIXEL]);
void _set_reset_bits_pwm();
void PWM_set_pwm_one_pixel(uint16_t pixel_idx, uint32_t rgb_val, uint8_t num_bits);
void _set_value_as_pwm(uint32_t pwm_data_idx, uint32_t value, uint8_t num_bits);

/* CALLBACKS */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim);


#endif /* _PWM_HPP_ */
