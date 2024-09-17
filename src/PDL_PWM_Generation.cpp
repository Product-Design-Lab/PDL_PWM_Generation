#include "PDL_PWM_Generation.h"

#include <Arduino.h>      // Includes Stream and other necessary Arduino core functions
#include <HardwarePWM.h>  // To control the PWM on nRF52840
#include <nrf_pwm.h>      // For nrf_pwm_clk_t
#include <math.h>         // To use math functions like log2() and ceil()

extern HardwarePWM HwPWM0;  // Use HwPWM0 for controlling PWM

#define PWM_CLOCK_FREQUENCY 16000000UL
#define MAX_TOP_VALUE (0x7FFF)

int8_t find_best_prescaler(float target_freq);
int calculate_top_value(int8_t prescaler_shift, float target_freq);
uint16_t calculate_duty_cycle_value(uint16_t top_value, float duty_cycle);
float calculate_frequency_error(float actual_freq, float target_freq);
float calculate_duty_cycle_error(float actual_duty_cycle, float target_duty_cycle);

// 1. Function to initialize the PWM on the specified pin using HwPWM0
int pwm_gen_init(uint8_t pin, float frequency, float duty_cycle) {
  // Step 1: Check if duty_cycle is between 0 and 1
  if (duty_cycle < 0.0f || duty_cycle > 1.0f) {
    Serial.printf("Error: Duty cycle must be between 0 and 1, provided: %f\n", duty_cycle);
    return -1;
  }

  // Step 2: Find the best prescaler for the target frequency
  int8_t prescaler_shift = find_best_prescaler(frequency);
  if (prescaler_shift == -1) {
    Serial.printf("Error: Invalid frequency: %f\n", frequency);
    return -1;
  }

  // Step 3: Calculate the top value for the given prescaler
  int top_value = calculate_top_value(prescaler_shift, frequency);
  if (top_value == -1) {
    Serial.println("Error: Invalid top value");
    return -1;
  }

  // Step 4: Calculate the duty cycle value
  uint16_t duty_cycle_value = calculate_duty_cycle_value((uint16_t)top_value, duty_cycle);

  // Step 5: Configure HwPWM0 for the specified pin
  HwPWM0.addPin(pin);  // Add the pin to the HwPWM0 module

  // Step 9: Start the PWM signal
  HwPWM0.begin();

  // Step 7: Set the maximum value for the PWM cycle using setMaxValue
  HwPWM0.setMaxValue(top_value);

  // Step 6: Set the prescaler using nrf_pwm_clk_t directly
  HwPWM0.setClockDiv((nrf_pwm_clk_t)(prescaler_shift));  // Use prescaler_shift directly with nrf_pwm_clk_t

  // Step 8: Write the duty cycle value to the pin
  HwPWM0.writePin(pin, duty_cycle_value, false);



  // Step 10: Calculate actual frequency and duty cycle for error analysis
  uint32_t prescaler = 1 << prescaler_shift;
  float actual_freq = (float)(PWM_CLOCK_FREQUENCY / (prescaler * top_value));
  float actual_duty_cycle = (float)duty_cycle_value / top_value;

  // Calculate errors
  float freq_error = calculate_frequency_error(actual_freq, frequency);
  float duty_cycle_error = calculate_duty_cycle_error(actual_duty_cycle, duty_cycle);

  // Print debug information
  Serial.println("PWM Initialization Success!");
  Serial.print("Prescaler: ");
  Serial.println(prescaler);
  Serial.print("Top Value: ");
  Serial.println(top_value);
  Serial.print("Duty Cycle Value: ");
  Serial.println(duty_cycle_value);
  Serial.print("Actual Frequency: ");
  Serial.println(actual_freq);
  Serial.print("Actual Duty Cycle: ");
  Serial.print(actual_duty_cycle * 100);
  Serial.println(" %");
  Serial.print("Frequency Error: ");
  Serial.println(freq_error);
  Serial.print("Duty Cycle Error: ");
  Serial.print(duty_cycle_error * 100);
  Serial.println(" %");

  return 0;  // Success
}

// Find the best prescaler
int8_t find_best_prescaler(float target_freq) {
  float ratio = (float)PWM_CLOCK_FREQUENCY / (float)(MAX_TOP_VALUE * target_freq);
  int8_t prescaler_shift = (int8_t)ceil(log2(ratio));  // Use log2() instead of log2f()
  if (prescaler_shift > 7 || prescaler_shift < 0) {
    return -1;
  }
  return prescaler_shift;
}

// Calculate the top value for the given prescaler
int calculate_top_value(int8_t prescaler_shift, float target_freq) {
  uint32_t prescaler = 1 << prescaler_shift;
  float clock_freq = (float)PWM_CLOCK_FREQUENCY / (float)prescaler;
  int top_value = (int)(clock_freq / target_freq);
  if (top_value > MAX_TOP_VALUE) {
    return -1;
  }
  return top_value;
}

// Calculate the duty cycle value
uint16_t calculate_duty_cycle_value(uint16_t top_value, float duty_cycle) {
  return (uint16_t)(duty_cycle * top_value);
}

// Calculate frequency error
float calculate_frequency_error(float actual_freq, float target_freq) {
  return fabsf(actual_freq - target_freq);
}

// Calculate duty cycle error
float calculate_duty_cycle_error(float actual_duty_cycle, float target_duty_cycle) {
  return fabsf(actual_duty_cycle - target_duty_cycle);
}
