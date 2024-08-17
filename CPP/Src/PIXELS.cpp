/**
 * @file PIXELS.c
 * @brief This file contains functions to interact with Pixel Strip and also initiliaze your strip
 * @author Aarjav jain
 * @date 2024-07-29
 */


// TODO: Optimise _set_pixel function (consider union and set all quick)
// TODO: Check if having a pixel_strip array for fast getting is worth it


#include "main.hpp"
#include "PWM.hpp"
#include "PIXELS.hpp"


/*  ============================ STATIC VARIABLE DEFINITIONS ============================ */

static uint8_t pixel_strip[NUM_PIXELS][LEDS_PER_PIXEL] = {0};


/*  ============================ FUNCTIONS DEFINITIONS ============================ */

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

void _dim_pixel(uint16_t pixel_idx, float decayRate)
{
    pixel_strip[pixel_idx][GREEN_IDX] *= decayRate;
    pixel_strip[pixel_idx][RED_IDX] *= decayRate;
    pixel_strip[pixel_idx][BLUE_IDX] *= decayRate;
}

void _min_pixel(uint16_t pixel_idx, uint8_t red, 
                                    uint8_t green, 
                                    uint8_t blue
)
{
    pixel_strip[pixel_idx][GREEN_IDX] = std::max(pixel_strip[pixel_idx][GREEN_IDX], green);
    pixel_strip[pixel_idx][RED_IDX] = std::max(pixel_strip[pixel_idx][RED_IDX], red);
    pixel_strip[pixel_idx][BLUE_IDX] = std::max(pixel_strip[pixel_idx][BLUE_IDX], blue);
}

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


void PIXELS_set_pixels_beat(uint16_t start_idx, uint16_t pixels, uint8_t red, 
                                                                 uint8_t green, 
                                                                 uint8_t blue
) 
{
    for (int i = start_idx; i < std::min((uint16_t)NUM_PIXELS, pixels); i+=3)
    {
        _set_pixel(i, red, green, blue);
    }
}

void PIXELS_set_pixels_min(uint16_t start_idx, uint16_t pixels, uint8_t red, 
                                                                 uint8_t green, 
                                                                 uint8_t blue
) 
{
    for (int i = start_idx; i < std::min((uint16_t)NUM_PIXELS, pixels); i++)
    {
        _min_pixel(i, red, green, blue);
    }
}

void PIXELS_dim_pixels_beat(uint16_t start_idx, uint16_t pixels, float decayRate) 
{
    for (int i = start_idx; i < std::min((uint16_t)NUM_PIXELS, pixels); i+=3)
    {
        _dim_pixel(i, decayRate);
    }
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
    if (start_pixel_idx > NUM_PIXELS || end_pixel_idx > NUM_PIXELS || start_pixel_idx < 0 || end_pixel_idx < 0)
    {
        return;
    }

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
 * @brief Rules: If bass flash red, if clap flash yellow, if hihat flash blue
 */
uint16_t* PIXELS_sound_react()
{
    if (beat_actions.do_bass) {            // Bass only = red
        state.red = 0;
        state.green = 0;
        state.blue = 50;

        state.decayRate = 0.4;
        state.redrawHihatCounter = 6;
        PIXELS_set_pixels_beat(BASS_START_IDX, NUM_PIXELS, state.red, state.green, state.blue);
        PIXELS_set_pixels_beat(CLAP_START_IDX, NUM_PIXELS, state.red, state.green, state.blue);
        PIXELS_set_pixels_beat(HIHAT_START_IDX, NUM_PIXELS, state.red, state.green, state.blue);
    } 
    else
    {
        PIXELS_dim_pixels_beat(BASS_START_IDX, NUM_PIXELS, 0.5);
        PIXELS_dim_pixels_beat(CLAP_START_IDX, NUM_PIXELS, 0.5);
        PIXELS_dim_pixels_beat(HIHAT_START_IDX, NUM_PIXELS, 0.5);
    }

    if (beat_actions.do_bass && beat_actions.do_clap) {        // Bass and clap = orange
        state.red = 50;
        state.green = 17;
        state.blue = 0;

        state.decayRate = 0.4;
        PIXELS_set_pixels_beat(CLAP_START_IDX, NUM_PIXELS, state.red, state.green, state.blue);
    } 
    else
    {
        PIXELS_dim_pixels_beat(CLAP_START_IDX, NUM_PIXELS, 0.5);
    }

    if (!beat_actions.do_bass && beat_actions.do_clap) {       // Clap only = yellow
        state.red = 50;
        state.green = 50;
        state.blue = 0;

        state.decayRate = 0.37;
        PIXELS_set_pixels_beat(CLAP_START_IDX, NUM_PIXELS, state.red, state.green, state.blue);
    } 
    else
    {
        PIXELS_dim_pixels_beat(CLAP_START_IDX, NUM_PIXELS, 0.37);
    }
    
    if (beat_actions.do_hihat && !beat_actions.do_bass && !beat_actions.do_clap && state.redrawHihatCounter == 0) {                               // Hihat only = dark blue
        state.red = 30;
        state.green = 0;
        state.blue = 0;

        state.decayRate = 0.2;
        PIXELS_set_pixels_beat(HIHAT_START_IDX, NUM_PIXELS, state.red, state.green, state.blue);
    } 
    else
    {
        state.redrawHihatCounter = std::max(state.redrawHihatCounter - 1, 0);
        PIXELS_dim_pixels_beat(HIHAT_START_IDX, NUM_PIXELS, 0.2);
    }

    PIXELS_set_pixels_min(0, NUM_PIXELS, 1, 0, 1);
    return PIXELS_send_pixels();
}

