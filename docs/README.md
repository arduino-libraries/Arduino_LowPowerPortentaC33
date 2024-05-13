
## Measurements
Here's an overview of the reduction in power usage that you can expect from this library. The screenshots below are taken from the nRF Power Profiler application using a Nordic PPK2 while running the blink sketch on the same board. 

#### Normal operating conditions
![](https://raw.githubusercontent.com/arduino-libraries/Arduino_Portenta_C33_LowPower/main/extras/results/normal_usage_blink.png)

#### Sleep Mode with peripherals off
![](https://raw.githubusercontent.com/arduino-libraries/Arduino_Portenta_C33_LowPower/main/extras/results/sleep_no_peripherals.png)

#### Deep Sleep Mode with peripherals off
![](https://raw.githubusercontent.com/arduino-libraries/Arduino_Portenta_C33_LowPower/main/extras/results/deep_sleep_no_peripherals.png)


#### Sleep Mode with peripherals on
![](https://raw.githubusercontent.com/arduino-libraries/Arduino_Portenta_C33_LowPower/main/extras/results/sleep_peripherals_on.png)

#### Deep Sleep Mode with peripherals on
![](https://raw.githubusercontent.com/arduino-libraries/Arduino_Portenta_C33_LowPower/main/extras/results/deep_sleep_peripherals_on.png)

## Usage
### Selecting a wakeup source
The wakeup source can be one of the deep-sleep enabled wakeup pins, and an RTC Alarm. You can select multiple pins and the RTC alarm to wake up the board. 

#### Wakeup Pins
This feature can be used when you want to wake up the board from external stimuli, such as sensors or user input. Some sensors have an interrupt pin that you can connect to one of the wakeup pins (eg: most motion sensors), while some output voltage on a pin, (eg: Passive Infrared Sensors or user buttons).

To select a wakeup pin just call `lowPower.setWakeupPin(<pin_number>, <direction>)`. The direction can be either **RISING** if you want to wake up when voltage is applied to a pin, or **FALLING** if you want to wake when no voltage is applied anymore. 

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
> Not all IRQs are created equal, the number of the IRQ represents it's priority. (IRQ0 being the highest priority and IRQ15 the lowest). Be careful when selecting your IRQ pin to make sure the board behaves as expected.

#### RTC Alarm
This feature is particularly useful when you want to set the board to wake up at specific times. You can use this in conjunction with the [RTC library](). 
To make your board wake up on an RTC alarm you simply need to call `lowPower.setWakeupRTC()` and it will enable that functionality. Check out [this example]() for more details about setting up the RTC. 


### Initiating sleep modes:
Use lowPower.sleep() for Sleep Mode and lowPower.deepSleep() for Deep Sleep Mode. Upon calling these methods the board will sleep until one of the wakeup events mentioned earlier arises. 

