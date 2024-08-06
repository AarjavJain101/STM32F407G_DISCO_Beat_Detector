/**
 * @file DETECTOR.c
 * @brief Contains functions to perform beat detection algorithm
 * @author Aarjav Jain
 * @date 2024-08-01
 */

#include "main.hpp"
#include "DETECTOR.hpp"
#include <vector>
#include "arm_math.h"
#include <stdio.h>

using namespace std;


/* =========== INITIALZE CALCULATION VARIABLES ===========*/

beat_actions_t          beat_actions = {
    .do_bass  = false,
    .do_clap  = false,
    .do_hihat = false
};

float                   fft_in_buf[FFT_SIZE];
float                   fft_out_buf[FFT_SIZE];
static arm_rfft_fast_instance_f32 fft_handler;                  // FFT handler
float                   real_fsample = SAMPLE_RATE;
vector<vector<float>>   history_freqs = vector<vector<float>>();

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


/* =========== FUNCTION DEFINITIONS ===========*/

/**
 * @brief Initialize the beat history vector
 * @note 3x3 vector for bass, clap, and hihat
 */
void _init_beat_history() 
{
    for (int i = 0; i < 3; i++) 
    {
        beat_history.push_back({0});
        for (int j = 0; j < 4; j++) 
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
 * @brief Get the energy of the clap sub bands
 * @return The energy of the clap sub bands
 */
float _get_clap_energy()
{
    return (1.2 * instant_energy[CLAP_RANGE_LOW]
            + 1.3 * instant_energy[CLAP_RANGE_LOW + 1]
            + 1.5 * instant_energy[CLAP_RANGE_LOW + 2]
            + 1.4 * instant_energy[CLAP_RANGE_LOW + 5] 
            + 1.6 * instant_energy[CLAP_RANGE_LOW + 6] 
            + 1.4 * instant_energy[CLAP_RANGE_LOW + 9] 
            + 1.6 * instant_energy[CLAP_RANGE_LOW + 10]) / 10;
}


/**
 * @brief Get the energy of the hihat sub bands
 * @return The energy of the hihat sub bands
 */
float _get_hihat_energy()
{
    return (1.3 * instant_energy[HIHAT_RANGE_LOW]
            + 1.7 * instant_energy[HIHAT_RANGE_LOW + 1]
            + 1.4 * instant_energy[HIHAT_RANGE_LOW + 2]
            + 1.2 * instant_energy[HIHAT_RANGE_LOW + 3]
            + 1.4 * instant_energy[HIHAT_RANGE_LOW + 4]) / 7;
}


/**
 * @brief Check if a beat occurred for each sub band
 * @note The algorithm is as follows:
 * 1. First normalize by dividing by max energy
 * 2. Then check if instant energy > threshold based on variance
 * 3. Fill a vector of true and false values for each sub band
 */
void _check_beat_in_chunk()
{
    /* ------------------- DO STEP 1 ------------------- */

    // Initialize variable to hold max energy for each sub band and number of histories
    vector<float> max_energies;
    max_energies.resize(TOTAL_SUB_BANDS, 0);

    vector<float> normalized_instant_energy;
    normalized_instant_energy.resize(TOTAL_SUB_BANDS, 0);

    vector<vector<float>> normalized_energy_history;
    normalized_energy_history.resize(energy_history.size(), vector<float>(TOTAL_SUB_BANDS, 0));

    int num_histories = HISTORY_SECONDS * (int)(SAMPLE_RATE / FFT_SIZE);

    // Looping through energy history
    for (int i = 0; i < TOTAL_SUB_BANDS; i++)
    {
        // Find the max energy for the specifc sub band first
        for (int j = 0; j < num_histories; j++)
        {
            max_energies[i] = max(max_energies[i], energy_history[j][i]);
        }

        // Now normalize each of the sub band values
        for (int j = 0; j < num_histories; j++)
        {
            normalized_energy_history[j][i] = energy_history[j][i] / max_energies[i];
        }
    }

    // Normalize each instant energy per sub band
    for (int i = 0; i < TOTAL_SUB_BANDS; i++)
    {
        float temp = instant_energy[i] / max_energies[i];
        normalized_instant_energy[i] = temp * temp;
    }

    /* ------------------- DO STEP 2 ------------------- */

    // Calculate the thresholds of each sub band based on energy history
    vector<float> thresholds;
    thresholds.resize(TOTAL_SUB_BANDS, 0);

    vector<float> average_energies;
    average_energies.resize(TOTAL_SUB_BANDS, 0);

    for (int i = 0; i < TOTAL_SUB_BANDS; i++)
    {
        for (int j = 0; j < num_histories; j++)
        {
            average_energies[i] += normalized_energy_history[j][i];
        }
        average_energies[i] = average_energies[i] / num_histories;

        for (int j = 0; j < num_histories; j++)
        {
            thresholds[i] += pow(normalized_energy_history[j][i] - average_energies[i], 2);
        }
        thresholds[i] = thresholds[i] / num_histories;
        thresholds[i] = -13.0 * thresholds[i] + 1.20;
    }

    /* ------------------- DO STEP 3 ------------------- */

    // Check if there is a beat for each sub band
    for (int i = 0; i < TOTAL_SUB_BANDS; i++)
    {
        float thresh0 = thresholds[0];
        float norm0 = normalized_instant_energy[0];
        float constan = norm0 - thresh0;
        // uint8_t bruh = 0;
        if (normalized_instant_energy[i] > thresholds[i] || normalized_instant_energy[i] > 1.15)
        {
            sub_band_beat[i] = true;
        }
        else
        {
            sub_band_beat[i] = false;
        }
    }
}


/**
 * @brief Compare the current beat with the beat history
 * @param curr_beat_energy: The energy of the current beat
 */
bool _compare_beat(float curr_beat_energy, int history_index)
{
    // Find the maximum energy of previously detected beats
    float max_energy = 0;
    for (int i = 0; i < beat_history[history_index].size(); i++)
    {
        max_energy = max(max_energy, beat_history[history_index][i]);
    }

    // Normalize all beat history values
    vector<float> norm_beat_history;
    norm_beat_history.resize(beat_history[history_index].size(), 0);
    for (int i = 0; i < beat_history[history_index].size(); i++)
    {
        norm_beat_history[i] = beat_history[history_index][i] / max_energy;
    }

    // Calculate the average of the beat history
    float average_energy = 0;
    for (int i = 0; i < beat_history[history_index].size(); i++)
    {
        average_energy += norm_beat_history[i];
    }
    average_energy = average_energy / beat_history[history_index].size();

    // Calculate the variance of the beat history
    float variance = 0;
    for (int i = 0; i < beat_history[history_index].size(); i++)
    {
        variance += pow(norm_beat_history[i] - average_energy, 2);
    }
    variance = variance / beat_history[history_index].size();

    // Check if current beat exceeds the threshold
    if (curr_beat_energy / max_energy > average_energy * variance * 0.64)
    {
        beat_history[history_index].erase(beat_history[history_index].begin());
        beat_history[history_index].push_back(curr_beat_energy);
        return true;
    }
    else
    {
        return false;
    }
}


/**
 * @brief Check if at least numTrue values are true in the input vector
 * @param sub_band_list: The input vector of booleans
 * @param numTrue: The number of true values required
 */
bool _check_true_values(vector<bool> sub_band_list, int numTrue)
{
    int trueCount = 0;
    for (int i = 0; i < sub_band_list.size(); i++)
    {
        if (sub_band_list[i])
        {
            trueCount++;
        }
    }

    if (trueCount >= numTrue)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/**
 * @brief Calculate the average of an array of integers
 */
float _get_average()
{
    float sum = 0;
    for (int i = 0; i < hihat_gap_array.size(); i++)
    {
        sum += hihat_gap_array[i];
    }
    return sum / hihat_gap_array.size();
}


/**
 * @brief Calculate the mode of an array of integers
 */
int _get_mode()
{
    int mode = 0;
    int maxCount = 0;
    int arraySize = hihat_gap_array.size();
    for (int i = 0; i < arraySize; i++)
    {
        int count = 0;
        for (int j = 0; j < arraySize; j++)
        {
            if (hihat_gap_array[j] == hihat_gap_array[i])
            {
                count++;
            }
        }
        if (count > maxCount)
        {
            maxCount = count;
            mode = hihat_gap_array[i];
        }
    }
    return mode;
}


/**
 * @brief Fills detection vectors and variables without impacting lights
 */
void _detect_history(int16_t* raw_audio_buffer)
{
    _int_to_float(raw_audio_buffer, fft_in_buf, FFT_SIZE);                      //  1. Prepare FFT input as floats
    _do_fft();                                                                  //  2. Take FFT of audio data
    _set_instant_energy();                                                      //  3. Calculate energy of sub bands

    energy_history.push_back(instant_energy);                                   //  4. Append energy history
        
    chunks_processed++;                                                         //  5. Increment chunks processed
}


void _update_next_iter()
{
    // Update the energy history
    int y = energy_history.size();
    energy_history.erase(energy_history.begin());
    int x = energy_history.size();
    energy_history.push_back(instant_energy);
    int z = energy_history.size();


    // Update the chunks processed
    chunks_processed++;
}


/**
 * @brief Detect beats forever
 */
void _detect_forever(int16_t* raw_audio_buffer)
{
    beat_actions.do_bass = false;
    beat_actions.do_clap = false;
    beat_actions.do_hihat = false;

    _int_to_float(raw_audio_buffer, fft_in_buf, FFT_SIZE);                      //  1. Prepare FFT input as floats
    _do_fft();                                                                  //  2. Take FFT of audio data
    _set_instant_energy();                                                      //  3. Calculate energy of sub bands
    _check_beat_in_chunk();                                                     //  4. Check for a beats in all subbands

    float bass_energy = instant_energy[0];                                      
    if (sub_band_beat[0])                                                       //  5. Accuretly check bass
    {
        if (chunks_processed - bass_chunk > 7)
        {
            beat_actions.do_bass = true;    
            bass_chunk = chunks_processed;
            // if (beat_history[0][3] > 0)
            // {
            //     if (_compare_beat(instant_energy[0], 0))
            //     {
                    // beat_actions.do_bass = true;
                    // bass_chunk = chunks_processed;
            //     }
            // }
            // else
            // {
            //     // Find the first index that is 0 in beat history
            //     int i = 0;
            //     while (beat_history[0][i] > 0)
            //     {
            //         i++;
            //     }
            //     beat_history[0][i] = instant_energy[0];
            //     beat_actions.do_bass = true;
            //     bass_chunk = chunks_processed;
            // }
        }
    }

     clap_energy = _get_clap_energy();                                           //  6. Accuretly check claps
     if (sub_band_beat[CLAP_RANGE_LOW]
         && sub_band_beat[CLAP_RANGE_LOW + 1]
         && sub_band_beat[CLAP_RANGE_LOW + 2]
         && sub_band_beat[CLAP_RANGE_LOW + 5]
         && sub_band_beat[CLAP_RANGE_LOW + 6]
         && sub_band_beat[CLAP_RANGE_LOW + 9]
         && sub_band_beat[CLAP_RANGE_LOW + 10])
     {
         if (chunks_processed - clap_chunk > 20)
         {
            //  clap_chunk = chunks_processed;
            //  beat_actions.do_clap = true;
            if (beat_history[1][3] > 0)
            {
                if (_compare_beat(clap_energy, 1))
                {
                    clap_chunk = chunks_processed;
                    beat_actions.do_clap = true;
                }
            }
            else
            {
                // Find the first index that is 0 in beat history
                int i = 0;
                while (beat_history[1][i] > 0)
                {
                    i++;
                }
                beat_history[1][i] = clap_energy;
            }
         }
     }


    hihat_energy = _get_hihat_energy();                //  6. Accuretly check hihats
    if (_check_true_values({sub_band_beat[HIHAT_RANGE_LOW], sub_band_beat[HIHAT_RANGE_LOW + 1], sub_band_beat[HIHAT_RANGE_LOW + 2], sub_band_beat[HIHAT_RANGE_LOW + 3], sub_band_beat[HIHAT_RANGE_LOW + 4]}, 1))
    {
        if (chunks_processed - hihat_chunk > 3)
        {
            if (beat_history[2][3] > 0)
            {
                if (_compare_beat(hihat_energy, 2))
                {
                    // Find the first index that is 0 in the gap array
                    int i = 0;
                    while (i < HIHAT_GAP_ARRAY_SIZE && hihat_gap_array[i] > 0)
                    {
                        i++;
                    }
                    
                    // If gap array not full yet then fill it
                    if (i < HIHAT_GAP_ARRAY_SIZE)
                    {
                        hihat_gap_array[i] = chunks_processed - hihat_chunk;
                    }                        
                    else
                    {
                        hihat_gap_average = _get_average();
                        hihat_gap_mode = _get_mode();

                        // Reset the hihat_gap_array
                        for (int i = 0; i < hihat_gap_array.size() - 1; i++)
                        {
                            hihat_gap_array[i] = hihat_gap_array[i + 1];
                        }
                        hihat_gap_array[hihat_gap_array.size() - 1] = chunks_processed - hihat_chunk;
                    }
                    hihat_chunk = chunks_processed;

                    if (hihat_gap_mode > 0 && fabs((hihat_gap_average / hihat_gap_mode) - 1) < 0.65 && hihat_gap_mode >= 7)
                    {
                        beat_actions.do_hihat = true;
                    }
                }
            }
            else
            {
                // Find the first index that is 0 in beat history
                int i = 0;
                while (beat_history[2][i] > 0)
                {
                    i++;
                }
                beat_history[2][i] = hihat_energy;
                beat_actions.do_hihat = true;
                hihat_chunk = chunks_processed;
            }
        }   
    }


    if (chunks_processed - bass_chunk > (int)(5 * SAMPLE_RATE / FFT_SIZE))               //  8. Reset bass and clap beat history if no bass for 5 seconds
    {
        for (int j = 0; j < 2; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                beat_history[j][i] = 0;
            }
        }
    }

    _update_next_iter();
}




/**
 * @brief Perform the detection algorithm
 * @param raw_audio_buffer: The raw audio buffer to perform detection on
 */
void DETECTOR_perform_detection(int16_t* raw_audio_buffer)
{
    if (_history_not_filled())
    {
        _detect_history(raw_audio_buffer);
    }
    else
    {
        _detect_forever(raw_audio_buffer);
    }
}
