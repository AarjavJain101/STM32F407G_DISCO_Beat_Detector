/**
 * @file PIXELS.h
 * @brief header file for PIXELS.cpp
 * @author Aarjav Jain
 * @date 2024-07-29
 */


/*  ============================ PIXEL STRIP ABSTRACTION ============================ 
 *  
 *  Each Pixel on the strip contains <LEDS_PER_PIXEL> PIXELs.
 *      - Represented by a uint8_t array of size <LEDS_PER_PIXEL> for optimal memory usage.
 *      - Defined in PIXELS.cpp
 *  
 *  The entire Pixel strip contains <NUM_PIXELS> Pixels.
 *      - Represented by a 2D uint8_t array of size <NUM_PIXELS> x <LEDS_PER_PIXEL>.
 *      - Defined in PIXELS.cpp
 */


#ifndef _PIXELS_HPP_
#define _PIXELS_HPP_

#include <stdint.h>
#include "CONSTANTS.hpp"


/*  ============== LED AND PIXEL STRIP CONSTANTS ============== */



/*  ============== RGBW COLOR CONSTANTS ============== */


/*  ============== PUBLIC FUNCTIONS DECLARATIONS ============== */
uint16_t* PIXELS_color_pixels(uint16_t start_pixel_idx, uint16_t end_pixel_idx, uint8_t red, 
                                                                                  uint8_t green, 
                                                                                  uint8_t blue
);
void PIXELS_set_pixels(uint16_t start_pixel_idx, uint16_t end_pixel_idx, uint8_t red, 
                                                                             uint8_t green, 
                                                                             uint8_t blue
);
uint16_t* PIXELS_send_pixels();
uint16_t* PIXELS_sound_react();

#endif /* _PIXELS_HPP_ */
