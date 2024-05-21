# 🔌 Portenta C33 Low Power Library

[![Check Arduino](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/check-arduino.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/check-arduino.yml) [![Compile Examples](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/compile-examples.yml) [![Spell Check](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/spell-check.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/spell-check.yml) [![Sync Labels](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/sync-labels.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/sync-labels.yml)


The Portenta C33 Low Power Library is a library designed to optimize power consumption for projects using the Portenta C33 boards, which are powered by the Renesas RA6M5 microcontroller. The library achieves this by providing access to various low power modes. These modes are essential for applications where power efficiency is crucial, such as in remote sensing, IoT devices, or battery-powered applications.


## Examples 
* [WakeFromGPIO](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/blob/main/examples/WakeFromGPIO/WakeFromGPIO.ino) - This example demonstrates how you can use [the GPIO pins](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/blob/main/docs/README.md#wakeup-pins) to wake your board from sleep.

* [WakeFromRTC](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/blob/main/examples/WakeFromRTC/WakeFromRTC.ino) - This example demonstrates how you can use the RTC to wake the board from sleep. 

* [TurnPeripheralsOff](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/blob/main/examples/TurnPeripheralsOff/TurnPeripheralsOff.ino) - This example extends the [WakeFromGPIO](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/blob/main/examples/WakeFromGPIO/WakeFromGPIO.ino) example by enabling the user to disable several power lanes to achieve the minimum power consumption of about ~60µA.


## 😴 Sleep Modes

### Sleep 
* **Function**: Reduces the microcontroller's power usage to about half of its normal consumption.
* **Effect**: Upon waking up from this mode, the execution of your program resumes exactly where it stopped. This is particularly useful for applications that require a quick resume with minimal power savings.
* **Wake-Up Triggers**: The board can be configured to wake up either from an RTC alarm or an external interrupt pin.
  
### Deep Sleep
* **Function**: Significantly reduces power usage to approximately 100uA (when all peripherals are off), making it ideal for long-term, battery-dependent operations.
* **Effect**: Unlike Sleep Mode, waking up from Deep Sleep Mode restarts the board, triggering the void setup() function. This behavior is suitable for scenarios where a full reset is acceptable or desired upon waking up.
* **Wake-Up Triggers**: The board can be configured to wake up either from an RTC alarm or an external interrupt pin.
  

## 📐 Measurements
Here's an overview of the reduction in power usage that you can expect from this library. The screenshots below are taken from the nRF Power Profiler application using a Nordic PPK2 while running the blink sketch on the same board. 

#### Normal operating conditions
![](https://raw.githubusercontent.com/arduino-libraries/Arduino_LowPowerPortentaC33/main/extras/results/normal_usage_blink.png)

#### Sleep Mode with peripherals off
![](https://raw.githubusercontent.com/arduino-libraries/Arduino_LowPowerPortentaC33/main/extras/results/sleep_no_peripherals.png)

#### Deep Sleep Mode with peripherals off
![](https://raw.githubusercontent.com/arduino-libraries/Arduino_LowPowerPortentaC33/main/extras/results/deep_sleep_no_peripherals.png)

#### Sleep Mode with peripherals on
![](https://raw.githubusercontent.com/arduino-libraries/Arduino_LowPowerPortentaC33/main/extras/results/sleep_peripherals_on.png)

#### Deep Sleep Mode with peripherals on
![](https://raw.githubusercontent.com/arduino-libraries/Arduino_LowPowerPortentaC33/main/extras/results/deep_sleep_peripherals_on.png)


> [!NOTE]  
> Note, this library reduces the power usage of the microcontroller, not the entire board and it's peripherals (Wi-Fi module for example). If you would like to reach the maximum power reduction possible for a Portenta C33 board, please check out the [TurnPeripheralsOff Example](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/blob/main/examples/TurnPeripheralsOff/TurnPeripheralsOff.ino) that makes use of the [Arduino_PF1550 Library](https://github.com/arduino-libraries/Arduino_PF1550)

For more information about this libraries check the [library documentation](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/blob/main/docs/README.md) or the [API Reference](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/blob/main/docs/api.md)
