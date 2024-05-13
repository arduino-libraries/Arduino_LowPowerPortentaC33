# Portenta C33 Low Power Library

[![Check Arduino](https://github.com/arduino-libraries/Arduino_Portenta_C33_LowPower/actions/workflows/check-arduino.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_Portenta_C33_LowPower/actions/workflows/check-arduino.yml) [![Compile Examples](https://github.com/arduino-libraries/Arduino_Portenta_C33_LowPower/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_Portenta_C33_LowPower/actions/workflows/compile-examples.yml) [![Spell Check](https://github.com/arduino-libraries/Arduino_Portenta_C33_LowPower/actions/workflows/spell-check.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_Portenta_C33_LowPower/actions/workflows/spell-check.yml) [![Sync Labels](https://github.com/arduino-libraries/Arduino_Portenta_C33_LowPower/actions/workflows/sync-labels.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_Portenta_C33_LowPower/actions/workflows/sync-labels.yml)


The Portenta C33 Low Power Library is a library designed to optimize power consumption for projects using the Portenta C33 boards, which are powered by the Renesas RA6M5 microcontroller. The library achieves this by providing access to various low power modes. These modes are essential for applications where power efficiency is crucial, such as in remote sensing, IoT devices, or battery-powered applications.


## Sleep Modes

### Sleep 
* **Function**: Reduces the microcontroller's power usage to about half of its normal consumption.
* **Effect**: Upon waking up from this mode, the execution of your program resumes exactly where it stopped. This is particularly useful for applications that require a quick resume with minimal power savings.
* **Wake-Up Triggers**: The board can be configured to wake up either from an RTC alarm or an external interrupt pin.
  
### Deep Sleep
* **Function**: Significantly reduces power usage to approximately 100uA (when all peripherals are off), making it ideal for long-term, battery-dependent operations.
* **Effect**: Unlike Sleep Mode, waking up from Deep Sleep Mode restarts the board, triggering the void setup() function. This behavior is suitable for scenarios where a full reset is acceptable or desired upon waking up.
* **Wake-Up Triggers**: The board can be configured to wake up either from an RTC alarm or an external interrupt pin.
  

> [!NOTE]  
> Note, this library reduces the power usage of the microcontroller, not the entire board and it's peripherals (Wi-Fi module for example). If you would like to reach the maximum power reduction possible for a Portenta C33 board, please use the [Arduino PowerManagement library](ttps://github.com/arduino-libraries/Arduino_PowerManagement) which allows you to turn off peripherals as well.

For more information about this libraries check the [library documentation](https://github.com/arduino-libraries/Arduino_Portenta_C33_LowPower/blob/main/docs/README.md) or the [API Reference](https://github.com/arduino-libraries/Arduino_Portenta_C33_LowPower/blob/main/docs/api.md)
