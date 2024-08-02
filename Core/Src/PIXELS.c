/**
 * @file PIXELS.c
 * @brief This file contains functions to interact with Pixel Strip and also initiliaze your strip
 * @author Aarjav jain
 * @date 2024-07-29
 */


// TODO: Optimise _set_pixel function (consider union and set all quick)
// TODO: Check if having a pixel_strip array for fast getting is worth it


#include "PIXELS.h"


/*  ============================ STATIC VARIABLE DEFINITIONS ============================ */

static uint8_t pixel_strip[NUM_PIXELS][LEDS_PER_PIXEL] = {0};


/*  ============================ FUNCTIONS DEFINITIONS ============================ */

/**
 * @brief Public function sets the color AND lights pixels in specified range to rgb 
 * @param start_pixel_idx: The index of the first pixel to set
 * @param end_pixel_idx: The index of the last pixel to set (non-inclusive)
 * @param red: The red value to set (0-255)
 * @param green: The green value to set (0-255)
 * @param blue: The blue value to set (0-255)
 */
uint16_t* PIXELS_color_pixels(uint16_t start_pixel_idx, uint16_t end_pixel_idx, uint8_t red, 
                                                                                  uint8_t green, 
                                                                                  uint8_t blue
) 
{
    PIXELS_set_pixels(start_pixel_idx, end_pixel_idx, red, green, blue);
    return PIXELS_send_pixels();
}


/**
 * @brief Public function Sets/prepares a range of pixels to be the same color
 * @param start_pixel_idx: The index of the first pixel to set
 * @param end_pixel_idx: The index of the last pixel to set (non-inclusive)
 * @param red: The red value to set (0-255)
 * @param green: The green value to set (0-255)
 * @param blue: The blue value to set (0-255)
 */
void PIXELS_set_pixels(uint16_t start_pixel_idx, uint16_t end_pixel_idx, uint8_t red, 
                                                                             uint8_t green, 
                                                                             uint8_t blue
) 
{
    for (uint16_t pixel_idx = start_pixel_idx; pixel_idx < end_pixel_idx; pixel_idx++) 
    {
        _set_pixel(pixel_idx, red, green, blue);
    }
}


/**
 * @brief Public function sends the pixel data to the PWM
 * @note: Uses the PWM abstraction to send the pixel data
 */
uint16_t* PIXELS_send_pixels() 
{
    return PWM_set_pwm_all_pixels(pixel_strip);
}



/**
 * @brief Private function Sets a single pixel to be a specific color
 * @param pixel_idx: The index of the pixel to set
 * @param red: The red value to set (0-255)
 * @param green: The green value to set (0-255)
 * @param blue: The blue value to set (0-255)
 */
void _set_pixel(uint16_t pixel_idx, uint8_t red, 
                                    uint8_t green, 
                                    uint8_t blue
) 
{
    pixel_strip[pixel_idx][GREEN_IDX] = green;
    pixel_strip[pixel_idx][RED_IDX] = red;
    pixel_strip[pixel_idx][BLUE_IDX] = blue;
}
