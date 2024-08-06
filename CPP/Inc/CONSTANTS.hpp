/**
 * @file CONSTANTS.hpp
 * @brief header file for all defines
 * @author Aarjav Jain
 * @date 2024-08-03
 */

/* =================== PWM TIMING/WIDTH CONSTANTS =================== */
#define PWM_PERIOD_COUNT              210
#define PWM_T1H_WS2812B               134
#define PWM_T0H_WS2812B               67
#define PWM_RESET_WS2812B             0

#define BITS_PER_LED                  8
#define BITS_PER_PIXEL                BITS_PER_LED * LEDS_PER_PIXEL
#define BITS_FOR_RESET                50            // requires > 50us of low signal to reset. 50 * 1.25 = 62.5us > 50us 


/* ========== DETECTOR CONSTANTS ========== */
#define DO_INVERSE_FFT              1
#define DO_FFT                      0
#define CHANNELS                    1
#define HISTORY_SECONDS             1
#define CLAP_RANGE_LOW              11
#define HIHAT_RANGE_LOW             27

#define TOTAL_SUB_BANDS             39 // Each sub band is a range of 5 * frequency resolution. it is ~230Hz wide and there are 39 of these

#define REAL                        0
#define IMAG                        1

#define HIHAT_GAP_ARRAY_SIZE        39


/* ========== AUDIO CONSTANTS ========== */
#define AUDIO_BUFFER_SIZE           8192
#define FFT_SIZE                    AUDIO_BUFFER_SIZE / 2
#define START_OF_FIRST_HALF         0
#define START_OF_SECOND_HALF        FFT_SIZE
#define NUM_FREQ_BINS               FFT_SIZE / 2
#define SAMPLE_RATE                 93750


/*  ============== LED AND PIXEL STRIP CONSTANTS ============== */
#define SET_COLOR(r, g, b)      ((g << 16U) | (r << 8U) | b)              // ORDER IS G-R-B because of WS2812B

#define NUM_PIXELS              150
#define LEDS_PER_PIXEL          3               // RGB only in WS2812B

#define GREEN_IDX               0
#define RED_IDX                 1
#define BLUE_IDX                2


#define BASS_START_IDX          0
#define CLAP_START_IDX          1
#define HIHAT_START_IDX         2
