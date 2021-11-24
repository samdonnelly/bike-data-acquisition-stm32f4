/*
 * File: adc_signal_filtering.c
 * Purpose: Implement a bufferworth filter on supplied data
 *
 * Date created: November 16, 2021
 * Author: Sam Donnelly 
 * 
 * Notes on this file and how to use it:
 *  - Go through each of the functions below and define information for each device using the 
 *    filter. There are comments saying where and how to define this information. 
 * 
 *  - In the 'butterworth_filter_setup()' function make sure to define the cutoff frequency (fc)
 *    and sampleing frequency (fs) correctly for each device. The cutoff frequency defines the 
 *    maximum frequency that will be let through the filter. The sample frequency is dependent 
 *    on how fast you run your code. It is recommended you throttle the code to a set sample 
 *    frequency or test the defined sample frequency until you get the correct results.
 * 
 *  - Make sure to call 'butterworth_filter(SETUP,...);' at the beginning of your code to 
 *    initialize all the filter equation constants so calculations are correct. After this 
 *    call 'butterworth_filter(FILTER,...);' each time you want to filter a device reading. 
 * 
 *  - The filtered signal values will have a slight phase delay from the raw signal values. 
 */

/*
 * ------------------------------------------------------------------------------------------------
 * Libraries and Header Files 
 * ------------------------------------------------------------------------------------------------
 */

#include "main.h"
#include "main_functions.h"
#include "adc_signal_filtering.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*
 * -----------------------------------------------------------------------------------------
 * Variables and Structures
 * -----------------------------------------------------------------------------------------
 */

typedef struct filter_eqn_const {
    // Filtered signal coefficients
    double a1;
    double a2;

    // Raw signal coefficients 
    double b0;
    double b1;
    double b2;
} filter_eqn_const;

typedef struct sensor_data {
    // Filtered signal coefficients
    double yn;
    double yn_1;
    double yn_2;

    // Raw signal coefficients
    double xn;
    double xn_1;
    double xn_2;
} sensor_data;

typedef struct sensor_list {
    filter_eqn_const sensor_const;
    sensor_data sensor_vals;
} sensor_list;


/*
 * -----------------------------------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------------------------------
 */ 

// Main filter function
double butterworth_filter(uint8_t function, uint8_t sensor_select, double sensor_raw) {
    // Define each sensor here 
    static sensor_list sensor;   // sensor_select == 0 

    switch(function) {
        case 0:
            // Define the constants 
            butterworth_filter_setup(&sensor);
            return 0;
        case 1:
            // Implement filter 
            switch (sensor_select) {
                // Add a case for each device used
                case 0:
                    // Calculate filtered value
                    sensor.sensor_vals.xn = sensor_raw;
                    butterworth_filter_equation(&sensor);
                    return sensor.sensor_vals.yn;
                
                default: 
                    // return unfiltered value
                    return sensor_raw;
        }
    }
}

// Generate butterworth filter constants based on cutoff frequency
void butterworth_filter_setup(sensor_list *sensor_setup) {

    // Set the intial conditions 
    // Call this function for each device 
    butterworth_filter_ic(sensor_setup);

    // Define the cutoff frequencies (Hz)
    // Add an appropriate fc_sensor for each device used. 
    const uint16_t fc_sensor = 5;

    // Define the programs sampling frequency (Hz)
    // (this depends on how fast you run your program loop)
    const uint16_t fs_sensor = 1000;

    // Constant 1 
    const double C1 = sqrt(2.0);

    // Constant 2 
    // Add a new C2 with the appropriate fc_sensor for each device. 
    const double C2_sensor = 1.0 / (M_PI*fc_sensor/fs_sensor);

    // Calculate constants for filter equation. 
    // Call this function with the appropriate C2_sensor for each device. 
    butterworth_filter_constants(sensor_setup, C1, C2_sensor);

}

// Set the initial conditions for the filter calculation 
void butterworth_filter_ic(sensor_list *ic) {
    ic->sensor_vals.yn_1 = 0.0;
    ic->sensor_vals.yn_2 = 0.0;
    ic->sensor_vals.xn_1 = 0.0;
    ic->sensor_vals.xn_2 = 0.0;
}

// Calculate constants for the filter equation 
void butterworth_filter_constants(sensor_list *constants, double c1, double c2) {
    constants->sensor_const.b0 = 1.0 / (1.0 + c1*c2 + c2*c2);
    constants->sensor_const.b1 = (constants->sensor_const.b0)*2.0;
    constants->sensor_const.b2 = (constants->sensor_const.b0);
    constants->sensor_const.a1 = (constants->sensor_const.b0)*2.0*(c2*c2 - 1.0);
    constants->sensor_const.a2 = (constants->sensor_const.b0)*(-1.0)*(1.0 - c1*c2 + c2*c2);
}

// Implement butterworth filter
void butterworth_filter_equation(sensor_list *eqn) {
    // Calculated filtered value 
    eqn->sensor_vals.yn = (eqn->sensor_const.a1)*(eqn->sensor_vals.yn_1) +
                          (eqn->sensor_const.a2)*(eqn->sensor_vals.yn_2) +
                          (eqn->sensor_const.b0)*(eqn->sensor_vals.xn)   +
                          (eqn->sensor_const.b1)*(eqn->sensor_vals.xn_1) +
                          (eqn->sensor_const.b2)*(eqn->sensor_vals.xn_2);
    
    // Redefine previous values for the next function call 
    eqn->sensor_vals.yn_2 = (eqn->sensor_vals.yn_1);
    eqn->sensor_vals.yn_1 = (eqn->sensor_vals.yn);
    eqn->sensor_vals.xn_2 = (eqn->sensor_vals.xn_1);
    eqn->sensor_vals.xn_1 = (eqn->sensor_vals.xn);

}

