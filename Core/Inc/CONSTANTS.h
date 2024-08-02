/**
 * @file CONSTANTS.h
 * @brief This file contains general constants used in the project
 * @author Aarjav Jain
 * @date 2024-07-29
 */


#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_


/*  ============== LED AND PIXEL STRIP CONSTANTS ============== */
#define SET_COLOR(r, g, b)      ((g << 16U) | (r << 8U) | b)              // ORDER IS G-R-B because of WS2812B

#define NUM_PIXELS              10
#define LEDS_PER_PIXEL          3               // RGB only in WS2812B

#define GREEN_IDX               0
#define RED_IDX                 1
#define BLUE_IDX                2


#endif /* _CONSTANTS_H_ */
