# Portenta C33 Low Power Library

This library provides access to the low power functionality of the Renesas RA6M5 microcontroller used in the Portenta C33 Boards and provides three sleep modes:
    *  **Automatic sleep** - This puts the board in a low-power state everytime `delay()` is called. While in this state, the board uses around 20% less power than it normally would.
    *  **Sleep** - This should bring the power usage to about half. When you wake up from sleep the execution of your program resumes from where it stopped.
    *  **Deep Seep** - In this mode the board can use as little as ~100uA and can wake up either from an RTC alarm or from an Intrerupt Pin. When waking up from this mode however, the board will be restarted, effectivelly running `void setup()` again. 

# Usage
## Select a wakeup source
This wakeup source can either a pin, or an RTC alarm.

### Wakeup Pins
To select a wakeup pin just call `lowPower.setWakeupPin(pin)`.

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



### RTC Alarm
To make your board wake up on an RTC alarm you simply need to call `lowPower.setWakeupRTC()` and it will enable that functionality .


## Put your board to sleep
To put your board to sleep or deepsleep simply call `lowPower.sleep()` or  `lowPower.deepSleep()`


## Known limitations
Automatic sleep doesn't currently work together with the other sleep modes.


