/**
 * @file PWM.c
 * @brief This file contains functions to interact with PWM and also initiliaze your PWM
 * @author Aarjav jain
 * @date 2024-07-29
 */


#include "PWM.h"


/*  ============================ STATIC VARIABLE DEFINITIONS ============================ */

uint16_t pwm_data[BITS_PER_PIXEL * NUM_PIXELS + BITS_FOR_RESET] = {0};
static bool is_data_sent = false;



/**
 * @brief Public function sets the entire pixel strip as PWM data
 * @param pixel_strip: The pixel strip to set
 */
uint16_t* PWM_set_pwm_all_pixels(uint8_t pixel_strip[NUM_PIXELS][LEDS_PER_PIXEL]) 
{
    for (uint16_t pixel_idx = 0; pixel_idx < NUM_PIXELS; pixel_idx++)                    // PWM for pixels on
    {
        uint32_t rgb_val = SET_COLOR(pixel_strip[pixel_idx][RED_IDX], 
                                     pixel_strip[pixel_idx][GREEN_IDX], 
                                     pixel_strip[pixel_idx][BLUE_IDX]
        );
        PWM_set_pwm_one_pixel(pixel_idx, rgb_val, BITS_PER_PIXEL);
    }

    _set_reset_bits_pwm();                                                               // PWM for reset

    return &(pwm_data[0]);
}


/**
 * @brief Private function to set the end >50us reset of the PWM for WS28128Bs
 */
void _set_reset_bits_pwm() 
{
    for (uint16_t pwd_data_idx = 0; pwd_data_idx < BITS_FOR_RESET; pwd_data_idx++) 
    {
        pwm_data[NUM_PIXELS * BITS_PER_PIXEL + pwd_data_idx] = PWM_RESET_WS2812B;
    }
}


/**
 * @brief Public function sets the PWM data for a specifc pxiel
 * @param pixel_idx: The index of the pixel to set
 * @param rgb_val: The RGB value to set
 * @param num_bits: The number of bits to set
 */
void PWM_set_pwm_one_pixel(uint16_t pixel_idx, uint32_t rgb_val, uint8_t num_bits) 
{
    uint32_t pwm_data_idx = pixel_idx * BITS_PER_PIXEL;
    _set_value_as_pwm(pwm_data_idx, rgb_val, BITS_PER_PIXEL);
}


/**
 * @brief Private function to set the value as its PWM form using WS2812B timings
 * @param pwm_data_idx: The index of the PWM data to set
 * @param value: The value to set
 * @param num_bits: The number of bits to set
 */
void _set_value_as_pwm(uint32_t pwm_data_idx, uint32_t value, uint8_t num_bits) 
{
    for (uint8_t curr_bit = 0; curr_bit < num_bits; curr_bit++) 
    {
        if (value & (1 << (num_bits - 1 - curr_bit))) 
            pwm_data[pwm_data_idx + curr_bit] = PWM_T1H_WS2812B;
        else 
            pwm_data[pwm_data_idx + curr_bit] = PWM_T0H_WS2812B;
    }
}
