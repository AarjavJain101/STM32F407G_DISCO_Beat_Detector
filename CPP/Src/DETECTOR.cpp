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


void DETECTOR_fft_init() 
{
    arm_rfft_fast_init_f32(&fft_handler, FFT_SIZE);
}


void _int_to_float(int16_t* in, float* out, uint16_t size) 
{
    for (uint16_t i = 0; i < size; i++) 
    {
        out[i] = (float) in[i];
    }
}

void DETECTOR_perform_detection(int16_t* raw_audio_buffer)
{
    /* ===== STEP 1: Perform FFT ===== */
    _int_to_float(raw_audio_buffer, fft_in_buf, FFT_SIZE);                  // Prepare FFT input
    arm_rfft_fast_f32(&fft_handler, fft_in_buf, fft_out_buf, DO_FFT);       // Perform FFT on data
}
