// Final Project Code
//
// Step 1: Set up timers to control lights and update variables in the program
// Step 2: Set up the input from the photcell
// Step 3: Combine the photocell input with the light cycle with interpts

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_timer.h"
#include "nrf_delay.h"
#include "nrfx_saadc.h"

#include "microbit_v2.h"

// Digital outputs
// SET BREAKOUT PINS
// These are GPIO pin numbers that can be used in nrf_gpio_* calls
// #define INTERSECTION_ONE_RED   EDGE_P13
// #define INTERSECTION_ONE_YELLOW EDGE_P14
// #define INTERSECTION_ONE_GREEN  EDGE_P15

// Digital inputs
// Breakout pin 16
// These are GPIO pin numbers that can be used in nrf_gpio_* calls
// #define SWITCH_IN EDGE_P16

// Analog inputs
// SET BREAKOUT PINS
// These are GPIO pin numbers that can be used in ADC configurations
// AIN1 is breakout pin 1. AIN2 is breakout pin 2.
// #define ANALOG_TEMP_IN  NRF_SAADC_INPUT_AIN1
//#define ANALOG_LIGHT_IN_INTERSECTION_ONE NRF_SAADC_INPUT_AIN2

// ADC channel configurations
// These are ADC channel numbers that can be used in ADC calls
//#define ADC_TEMP_CHANNEL  0
//#define ADC_LIGHT_CHANNEL 1

// Global variables
APP_TIMER_DEF(intersection_one_timer);

//// These are going to store values associated with the first intersection
// This light is going to spend 5 seconds on green, 2 on yellow and 7 on red
// These booleans are going to store the state of the light, the light is going
// to start at green.  The last boolean is going to be a state change so the
// change led fucntion is only called if there was a change in light color
bool intersection_one_is_red = false;
bool intersection_one_is_yellow = false;
bool intersection_one_is_green = false;
bool intersection_one_state_change = true;

// This is going to store the current timing cycle of the light, it wil 'overflow'
// after 14 seconds
int intersection_one = 0;

// This is going to change the light color for the lights on the first intersection
void intersection_one_set_lights(bool red, bool yellow, bool green) {
    // clear for on, set if off
    if (red) {
        //nrf_gpio_pin_clear(INTERSECTION_ONE_RED);
        printf("Intersection has a red light!");
    }
    else {
        //nrf_gpio_pin_set(INTERSECTION_ONE_RED);
    }

    if (yellow) { 
        //nrf_gpio_pin_clear(INTERSECTION_ONE_YELLOW);
        printf("Intersection has a yellow light!");
    }
    else {
        //nrf_gpio_pin_set(INTERSECTION_ONE_YELLOW);
    }

    if (green) {
        //nrf_gpio_pin_clear(INTERSECTION_ONE_GREEN);
        printf("Intersection has a green light!");
    }
    else {
        //nrf_gpio_pin_set(INTERSECTION_ONE_GREEN);
    }
}

// This is going to be a callback function associated with the timer for the first
// intersection
static void intersection_one_status(void* _unused) {
    // The first goal is to change the LEDs if there was a state change
    if (intersection_one_state_change) { 
        intersection_one_set_lights(intersection_one_is_red, 
        intersection_one_is_yellow, intersection_one_is_green); 

        // There will be no state change now
        intersection_one_state_change = false;
    }

    // The next goal is to update the timing cycle for the intersection and then
    // change the state if needed, if the value is over 14 set it back to 0
    intersection_one += 1;

    if (intersection_one > 14) {
        intersection_one = 0;
    }

    /// The last goal is to use the new value to see if there should be a light change next time
    // When it changes from red to green next cycle
    if (intersection_one == 0) {
        intersection_one_state_change = true;
        intersection_one_is_red = false;
        intersection_one_is_green = true;
    }

    // When it changes from green to yellow
    if (intersection_one == 5) {
        intersection_one_state_change = true;
        intersection_one_is_green = false;
        intersection_one_is_yellow = true;
    }

    // When it changes from yellow to red
    if (intersection_one == 7) {
        intersection_one_state_change = true;
        intersection_one_is_yellow = false;
        intersection_one_is_red = true;
    }
}

// Function prototypes
static void gpio_init(void);
static void adc_init(void);
static float adc_sample_blocking(uint8_t channel);

static void saadc_event_callback(nrfx_saadc_evt_t const* _unused) {
  // don't care about saadc events
  // ignore this function
}

// Set up the LEDs for the first intersection
static void gpio_init(void) {
  // Initialize output pins for the first intersection
//   nrf_gpio_cfg_output(INTERSECTION_ONE_RED);
//   nrf_gpio_cfg_output(INTERSECTION_ONE_YELLOW);
//   nrf_gpio_cfg_output(INTERSECTION_ONE_GREEN);

//   // Set LEDs off initially
//   nrf_gpio_pin_set(INTERSECTION_ONE_RED);
//   nrf_gpio_pin_set(INTERSECTION_ONE_YELLOW);
//   nrf_gpio_pin_set(INTERSECTION_ONE_GREEN);

  // Initialize input pin
  // TODO
}

// static void adc_init(void) {
//   // Initialize the SAADC
//   nrfx_saadc_config_t saadc_config = {
//     .resolution = NRF_SAADC_RESOLUTION_12BIT,
//     .oversample = NRF_SAADC_OVERSAMPLE_DISABLED,
//     .interrupt_priority = 4,
//     .low_power_mode = false,
//   };
//   ret_code_t error_code = nrfx_saadc_init(&saadc_config, saadc_event_callback);
//   APP_ERROR_CHECK(error_code);

//   // Initialize temperature sensor channel - DONT NEED
//   nrf_saadc_channel_config_t temp_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_TEMP_IN);
//   error_code = nrfx_saadc_channel_init(ADC_TEMP_CHANNEL, &temp_channel_config);
//   APP_ERROR_CHECK(error_code);

//   // Initialize light sensor channel
//   nrf_saadc_channel_config_t light_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_LIGHT_IN);
//   error_code = nrfx_saadc_channel_init(ADC_LIGHT_CHANNEL, &light_channel_config);
//   APP_ERROR_CHECK(error_code);
// }

// static float adc_sample_blocking(uint8_t channel) {
//   // read ADC counts (0-4095)
//   // this function blocks until the sample is ready
//   int16_t adc_counts = 0;
//   ret_code_t error_code = nrfx_saadc_sample_convert(channel, &adc_counts);
//   APP_ERROR_CHECK(error_code);

//   // convert ADC counts to volts
//   // 12-bit ADC with range from 0 to 3.6 Volts
//   // TODO

//   // return voltage measurement
//   return 0.0;
// }


int main(void) {
  printf("Board started!\n");
  
  // initialize GPIO
  //gpio_init();

  // initialize ADC
  //adc_init();

  // initialize app timers
  app_timer_init();
  app_timer_create(&intersection_one_timer, APP_TIMER_MODE_REPEATED, intersection_one_status);

  // Start the timer and call it every second
  app_timer_start(intersection_one_timer, 32768, NULL);

  // loop forever
  while (1) {
    // Don't put any code in here. Instead put periodic code in `sample_timer_callback()`
    nrf_delay_ms(1000);
  }
}

// Make a function that is called when the interrupt handler reads the starting switch was flipped

// make separate files for each intersection
// make file for start and end conditions