# 🔌 Portenta C33 Low Power Library

[![Check Arduino](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/check-arduino.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/check-arduino.yml) [![Compile Examples](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/compile-examples.yml) [![Spell Check](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/spell-check.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/spell-check.yml) [![Sync Labels](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/sync-labels.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_LowPowerPortentaC33/actions/workflows/sync-labels.yml)

The Portenta C33 Low Power Library is a library designed to optimize power consumption for projects using the Portenta C33 boards, which are powered by the Renesas RA6M5 microcontroller. The library achieves this by providing access to various low power modes. These modes are essential for applications where power efficiency is crucial, such as in remote sensing, IoT devices, or battery-powered applications.

## Examples 
* [WakeFromPin](./examples/WakeFromPin/WakeFromPin.ino) - This example demonstrates how you can use [the GPIO pins](#wake-up-pins) to wake your board from sleep.
* [WakeFromRTC](./examples/WakeFromRTC/WakeFromRTC.ino) - This example demonstrates how you can use the RTC to wake the board from sleep. 

## 💤 Sleep Modes

### 🥱 Sleep 
* **Function**: Reduces the microcontroller's power usage to about half of its normal consumption.
* **Effect**: Upon waking up from this mode, the execution of your program resumes exactly where it stopped. This is particularly useful for applications that require a quick resume with minimal power savings.
* **Wake-Up Triggers**: The board can be configured to wake up either from an RTC alarm or an external interrupt pin.
  
### 😴 Deep Sleep
- **Function**: Significantly reduces power usage to approximately 100uA (when all peripherals are off), making it ideal for long-term, battery-dependent operations.
- **Effect**: Unlike Sleep Mode, waking up from Deep Sleep Mode restarts the board, triggering the `setup()` function. This behavior is suitable for scenarios where a full reset is acceptable or desired upon waking up. The application state may need to be persisted before going to sleep.
- **Wake-Up Triggers**: The board can be configured to wake up either from an RTC alarm or an external interrupt pin.


## 📐 Measurements
Here's an overview of the reduction in power usage that you can expect from this library. 

| Mode             | Peripherals  | Current     |
|------------------|--------------|-------------|
| Normal Operation | Off          | 41.37mA     |
| Sleep            | On           | 18.26mA     |
| Sleep            | Off          | 7.02mA      |
| Deep Sleep       | On           | 11.57mA     |
| Deep Sleep       | Off          | **58.99uA** |

For more information about these measurements check out [this page](./docs/)

## 💻 Usage
### Selecting a Wake-Up Source
The wake-up source can be one of the deep-sleep enabled wake-up pins, and an RTC Alarm. You can select multiple pins and the RTC alarm to wake up the board. 

#### 📍 Wake-Up Pins
This feature can be used when you want to wake up the board from external stimuli, such as sensors or user input. Some sensors have an interrupt pin that you can connect to one of the wake-up pins (eg: most motion sensors), while others output a voltage on a pin when triggered (eg: Passive Infrared Sensors or user buttons).

To set up a wake-up pin call `lowPower.setWakeupPin(<pin_number>, <direction>)`. The direction can be either `RISING` if you want to wake up when voltage is applied to a pin, or `FALLING` if you want to wake when no voltage is applied anymore. 

Here is a list of the usable interrupts: 

| Arduino Pin | MCU PIN | IRQ     |
|-------------|---------|---------|
| A0          | P006    | IRQ11|
| A1          | P005    | IRQ10|
| A2          | P004    | IRQ9 |
| A3          | P002    | IRQ8 |
| A4          | P001    | IRQ7 |
| A5          | P015    | IRQ13|
| D4          | P401    | IRQ5 |
| D7          | P402    | IRQ4 |

> [!IMPORTANT]  
> Not all IRQs are created equal. The number of the IRQ represents it's priority. (`IRQ0` being the highest priority and `IRQ15` the lowest). Be careful when selecting your IRQ pin to make sure the board behaves as expected.

#### ⏰ RTC Alarm
This feature is particularly useful when you want to set the board to wake up at specific times. You can use this in conjunction with the [RTC library](https://github.com/arduino/ArduinoCore-renesas/tree/main/libraries/RTC). 
To make your board wake up on an RTC alarm you simply need to call `lowPower.setWakeUpAlarm(time)` or `lowPower.setWakeUpAlarm(hours, minutes, seconds)`. Check out this [example](./examples/WakeFromRTC/) for more details about setting up the RTC.


### 🛏️ Initiating sleep modes
Use `lowPower.sleep()` to enter the Sleep Mode and `lowPower.deepSleep()` to enter the Deep Sleep Mode. Upon calling these methods the board will sleep until one of the wake-up events mentioned earlier arises. 

For more information on how to use this library check the [API Reference](./docs/api.md)
