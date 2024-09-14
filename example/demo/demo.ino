#include <Arduino.h>
#include <PDL_PWM_Generation.h>
#include "Adafruit_TinyUSB.h"

void setup()
{
    // Initialize USB serial communication
    Serial.begin(115200);

    while (!Serial && millis() < 5000)
    {
        delay(100);
    }

    // Generate a PWM signal on pin LED_RED at 500 Hz with a 75% duty cycle
    if (pwm_gen_init(LED_RED, 4.0f, 0.5f) == -1)
    {
        Serial.println("PWM initialization failed!");
    }
    else
    {
        Serial.println("PWM initialization successful!");
    }
}

void loop()
{
   yield();
}