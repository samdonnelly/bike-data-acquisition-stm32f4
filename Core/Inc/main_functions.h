/**
 * @file main_functions.h
 * 
 * @author Sam Donnelly
 * 
 * @brief Header file for main_functions.c 
 * 
 * @details 
 * 
 * @version 0.1
 * @date 2022-01-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef MAIN_FUNCTIONS_H_
#define MAIN_FUNCTIONS_H_


//======================================================================================
// Includes

#include "includes.h"

//======================================================================================


//======================================================================================
// Macros

#define NUM_STATES 9
#define START_TIME_LIMIT 1000

//======================================================================================


//======================================================================================
// Enums

/**
 * @brief 
 * 
 */
typedef enum {
    STARTUP_STATE,
    IDLE_STATE,
    MODE_SET_STATE,
    SYSTEM_CHECK_STATE,
    SENSOR_CAL_STATE,
    PRE_RECORDING_STATE,
    RECORDING_STATE,
    POST_RECORDING_STATE,
    FAULT_STATE,
    LOW_POWER_MODE_STATE
} bda_states_t;



typedef enum {
    TOGGLE_MODE_SET,
    TOGGLE_SENSOR_CAL,
    TOGGLE_SYS_CHECK
} toggle_button_modes_t;


typedef enum {
    FALSE,
    TRUE
} bit_set_t;


//======================================================================================


//======================================================================================
// Structures 

typedef struct bda_trackers_s {
    // State of the state machine
    bda_states_t bda_state;

    // Fork pot 1 
    uint16_t fork_pot_1;

    // Fork pot 2 
    uint16_t fork_pot_2;

    // Shock pot 
    uint16_t shock_pot;

    // Steering angle
    uint16_t steering_angle;

    // Wheel speed
    uint16_t wheel_speed;

    // x-axis acceleration 
    int16_t x_accel;

    // y-axis acceleration 
    int16_t y_accel;

    // z-axis acceleration 
    int16_t z_accel;

    // x-axis rotation 
    int16_t x_rotation;

    // y-axis rotation
    int16_t y_rotation;

    // z-axis rotation 
    int16_t z_rotation;

} bda_trackers_t;

//======================================================================================


//======================================================================================
// Function pointers

typedef void (*bda_state_functions_t)(
    bda_trackers_t *trackers);

//======================================================================================


//======================================================================================
// Function Prototypes 

/**
 * @brief Psudo main function 
 * 
 */
void main_function(void);

/**
 * @brief 
 * 
 */
void bda_init(void);

/**
 * @brief Get the record input object
 * 
 */
uint8_t get_record_input(void);

/**
 * @brief Get the selector input object
 * 
 */
uint8_t get_selector_input(void);

/**
 * @brief Get the toggle input object
 * 
 */
uint8_t get_toggle_input(void);

/**
 * @brief 
 * 
 */
uint8_t fault_checks(void);

/**
 * @brief 
 * 
 */
void startup_state(bda_trackers_t *trackers);

/**
 * @brief 
 * 
 */
void idle_state(bda_trackers_t *trackers);

/**
 * @brief 
 * 
 */
void mode_set_state(bda_trackers_t *trackers);

/**
 * @brief 
 * 
 */
void system_check_state(bda_trackers_t *trackers);

/**
 * @brief 
 * 
 */
void sensor_calibration_state(bda_trackers_t *trackers);

/**
 * @brief 
 * 
 */
void pre_recording_state(bda_trackers_t *trackers);

/**
 * @brief 
 * 
 */
void recording_state(bda_trackers_t *trackers);

/**
 * @brief 
 * 
 */
void post_recording_state(bda_trackers_t *trackers);

/**
 * @brief 
 * 
 */
void fault_state(bda_trackers_t *trackers);

/**
 * @brief 
 * 
 */
void low_power_mode_state(bda_trackers_t *trackers);


/**
 * @brief 
 * 
 */
void normal(void);


/**
 * @brief Print to LCD 
 * 
 * @param pos1 
 * @param pos2 
 * @param pos3 
 * @param str 
 * @param index 
 * @param val 
 * @param data 
 */
void print_data(
    uint8_t pos1, 
    uint8_t pos2, 
    uint8_t pos3, 
    char str[], 
    uint8_t index, 
    float val,
    int data[]);


/**
 * @brief Prepare to calibrate the accelerometer
 * 
 */
void accel_cal_prep(void);


/**
 * @brief Send information over UART
 * 
 * @param string 
 */
void send_uart(char *string);

//======================================================================================


#endif   // MAIN_FUNCTIONS_H_
