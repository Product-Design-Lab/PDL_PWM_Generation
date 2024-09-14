# PDL_PWM_Generation

A library for generating and controlling Pulse Width Modulation (PWM) signals on the nRF52 family of microcontrollers, specifically designed for the nRF52840 using the HardwarePWM library. This library allows you to initialize and configure PWM on any GPIO pin with a specific frequency and duty cycle.

## Features
- Simple API for configuring PWM frequency and duty cycle.
- Supports hardware PWM on the nRF52 platform (e.g., nRF52840).
- Error reporting for out-of-range frequency and duty cycle values.
- Provides actual PWM frequency and duty cycle after initialization for accuracy reporting.

## Requirements
- **Hardware**: nRF52-based microcontroller (such as nRF52840).
- **Software**: 
  - Arduino IDE or PlatformIO.
  - `HardwarePWM` and `nrf_pwm` libraries (included in the Nordic SDK).

## Installation

### Using Arduino IDE
1. Download the latest release of the library from [GitHub](https://github.com/yourusername/PDL_PWM_Generation).
2. In the Arduino IDE, go to `Sketch -> Include Library -> Add .ZIP Library` and select the downloaded `.zip` file.
3. The library is now installed and ready to use.

### Using PlatformIO
Add the following dependency to your `platformio.ini` file:

```ini
lib_deps =
    yourusername/PDL_PWM_Generation
```

## API

#### `int pwm_gen_init(uint8_t pin, float frequency, float duty_cycle)`
Initializes PWM on a given pin with a specified frequency and duty cycle.

- `pin`: The GPIO pin number where the PWM signal will be generated.
- `frequency`: The desired frequency of the PWM signal (in Hz).
- `duty_cycle`: The duty cycle of the PWM signal (0.0 to 1.0).
  
Returns `0` on success, or `-1` if an error occurred.

#### Error Reporting
- If the duty cycle is out of range (below 0.0 or above 1.0), an error message is printed via `Serial` and the function returns `-1`.
- If the frequency is not valid, an error is printed, and the function returns `-1`.

### Debug Information

After initializing the PWM signal, the library prints debugging information about the prescaler, top value, actual frequency, and duty cycle to the Serial Monitor for precise control and troubleshooting.
F