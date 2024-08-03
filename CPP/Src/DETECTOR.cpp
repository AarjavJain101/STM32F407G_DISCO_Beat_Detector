/**
 * @file DETECTOR.c
 * @brief Contains functions to perform beat detection algorithm
 * @author Aarjav Jain
 * @date 2024-08-01
 */

#include "DETECTOR.hpp"

using namespace std;


float fft_in_buf[FFT_SIZE];
float fft_out_buf[FFT_SIZE];
static arm_rfft_fast_instance_f32 fft_handler;                  // FFT handler
float real_fsample = SAMPLE_RATE;
vector<vector<float>> history_freqs = vector<vector<float>>();



/* =========== INITIALZE CALCULATION VARIABLES ===========*/

int                     chunks_processed = 0;         // Number of chunks processed
vector<float>           instant_energy;               // Energy for each of 39 sub bands as a vector
vector<vector<float>>   energy_history;               // Energy history for HISTORY_SECONDS taken at SAMPLE_RATE / FFT_SIZE times per second
vector<bool>            sub_band_beat;                // Beat tracking variable for each sub band (true if there is a beat otherwise false)
vector<vector<float>>   beat_history;                 // Beat history for successfully detected bass and claps and hihats


// Chunk/time trackers and energy for bass, claps, and hihats
int                     bass_chunk = 0;
int                     clap_chunk = 0;
float                   clap_energy = 0;
int                     hihat_chunk = 0;
float                   hihat_energy = 0;
int                     hihat_gap_mode = 0;
float                   hihat_gap_average = 0;
vector<int>             hihat_gap_array;


/* DATA VIEWERS DELETE LATER */
float bass_sub_band = 0;



// vector<float> freqs = vector<float>(AUDIO_BUFFER_SIZE / 4);
// uint16_t freqpoint = 0;
// int offset = 150; 

// //calculate abs values and linear-to-dB
// for (int i=0; i<AUDIO_BUFFER_SIZE / 2; i=i+2) {
//         if (freqpoint == AUDIO_BUFFER_SIZE / 4) {
//               int x = 0;
//         }
// 	freqs[freqpoint] = complexABS(fft_out_buf[i], fft_out_buf[i+1]);
// 	if (freqs[freqpoint]<0.0) freqs[freqpoint]=0.0;
// 	freqpoint++;
// }


/**
 * @brief Initialize the beat history vector
 * @note 3x3 vector for bass, clap, and hihat
 */
void _init_beat_history() 
{
    for (int i = 0; i < 3; i++) 
    {
        beat_history.push_back({0});
        for (int j = 0; j < 3; j++) 
        {
            beat_history[i].push_back(0);
        }
    }
}


/**
 * @brief Initialize the instant energy vector
 */
void _init_instant_energy() 
{
    instant_energy.resize(TOTAL_SUB_BANDS, 0);
}


/**
 * @brief Initialize the sub band beat vector
 * @note All sub bands are set to true initially
 */
void _init_sub_band_beat() 
{
    sub_band_beat.resize(TOTAL_SUB_BANDS, true);
}


/**
 * @brief Initialize the hihat gap array
 * @note Used to determine hihat pattern
 */
void _init_hihat_gap_array()
{
    hihat_gap_array.resize(HIHAT_GAP_ARRAY_SIZE, 0);
}


/**
 * @brief Initializes all detection variables
 */
void _init_detection_variables() 
{
    _init_instant_energy();
    _init_sub_band_beat();
    _init_beat_history();
    _init_hihat_gap_array();
}


/**
 * @brief Initialize the FFT handler and variables
 */
void DETECTOR_init() 
{
    arm_rfft_fast_init_f32(&fft_handler, FFT_SIZE);
    _init_detection_variables();
}


/**
 * @brief Converts an array of integers to an array of floats for FFT input
 * @param in: The input array of integers
 * @param out: The output array of floats
 * @param size: The size of the arrays
 */
void _int_to_float(int16_t* in, float* out, uint16_t size) 
{
    for (uint16_t i = 0; i < size; i++) 
    {
        out[i] = (float) in[i];
    }
}


/**
 * @brief Checks if the <HISTORY_SECONDS> has elapsed
 */
bool _history_not_filled()
{
    return chunks_processed < (HISTORY_SECONDS * (int)(SAMPLE_RATE / FFT_SIZE));
}



/**
 * @brief Calculates the energy of each sub band
 * @note Each sub band is 230Hz wide
 * @note The energy is calculated by summing the cube of the real part of the FFT
 */
void _set_instant_energy()
{
    for (int i = 0; i < TOTAL_SUB_BANDS; i++)
    {
        float energy = 0;
        for (int j = i * 5; j < (i + 1) * 5; j++)
        {
            float abs_val = abs(fft_out_buf[2 * j]);
            energy += abs_val * abs_val * abs_val;
        }
        energy = energy / 5.0;
        instant_energy[i] = energy;
    }
}


/**
 * @brief Perform the FFT on the audio data using cmsis f32 fast rfft
 */
void _do_fft()
{
    arm_rfft_fast_f32(&fft_handler, fft_in_buf, fft_out_buf, DO_FFT);        
}


/**
 * @brief Fills detection vectors and variables without impacting lights
 */
void _detect_history(int16_t* raw_audio_buffer)
{
    if (chunks_processed > 30) 
    {
        _int_to_float(raw_audio_buffer, fft_in_buf, FFT_SIZE);        //  1. Prepare FFT input as floats
        _do_fft();                                                    //  2. Take FFT of audio data
        _set_instant_energy();                                        //  3. Calculate energy of sub bands

    //    energy_history.push_back(instant_energy);                     //  4. Append energy history

        bass_sub_band = instant_energy[0];                            //  DELETE LATER
        
    }
    chunks_processed++;                                           //  5. Increment chunks processed
}


/**
 * @brief Perform the detection algorithm
 * @param raw_audio_buffer: The raw audio buffer to perform detection on
 */
void DETECTOR_perform_detection(int16_t* raw_audio_buffer)
{
    _detect_history(raw_audio_buffer);
    if (_history_not_filled())
    {
    }
    else
    {
        return;
    }
}
