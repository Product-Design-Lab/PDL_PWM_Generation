#pragma once

#include <stdint.h>

// Function to initialize the PWM on the specified pin with the given frequency and duty cycle
int pwm_gen_init(uint8_t pin, float frequency, float duty_cycle);


