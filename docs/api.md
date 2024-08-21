# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class ` [`LowPower`](#class_low_power) | Provides functions for managing power and implementing sleep modes on an Arduino device.

# class `LowPower` <a id="class_low_power" class="anchor"></a>

Provides functions for managing power and implementing sleep modes on an Arduino device.

This class allows the user to put the device into different low-power states and configure wake-up sources such as external pins or timers.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
| [`LowPower`](#class_low_power_1a096a38a40767b27b681330a514fd4e48) | Constructor for [LowPower](#class_low_power) class. Initializes the low power and sleep configurations. |
| [`sleep`](#class_low_power_1a351563f87f8d1f118e9d77201fd59a55) | Puts the device into a standard sleep mode. This sleep mode consumes less power than the active mode but more than the deep sleep mode. The device will resume execution from the point where it entered the sleep mode. |
| [`deepSleep`](#class_low_power_1ada3410c10d04970e0898eb5f7f36ce55) | Puts the device into a deep sleep mode. The device consumes the least power in this mode but will reset when it wakes up effectively running the setup() function again. |
| [`enableWakeupFromPin`](#class_low_power_1a9ee83fbc2b670ac9490cd179b2e08138) | Enables wake-up of the device from a specified pin (A0, A1, A2, A3, A4, A5, D4, D7 )  |
| [`setWakeUpAlarm`](#class_low_power_1ae4bf7b3a270d1a54a00a527191031334) | Enables wake-up of the device based on the Real-Time Clock (RTC). The device will wake up at the specified time.  |
| [`setWakeUpAlarm`](#class_low_power_1add42684056e6bc5573fa500fc581b9b8) | Enables wake-up of the device based on the Real-Time Clock (RTC). The device will wake up at the specified time.  |

## Members

### `LowPower` <a id="class_low_power_1a096a38a40767b27b681330a514fd4e48" class="anchor"></a>

```cpp
LowPower()
```

Constructor for [LowPower](#class_low_power) class. Initializes the low power and sleep configurations.
<hr />

### `sleep` <a id="class_low_power_1a351563f87f8d1f118e9d77201fd59a55" class="anchor"></a>

```cpp
void sleep()
```

Puts the device into a standard sleep mode. This sleep mode consumes less power than the active mode but more than the deep sleep mode. The device will resume execution from the point where it entered the sleep mode.
<hr />

### `deepSleep` <a id="class_low_power_1ada3410c10d04970e0898eb5f7f36ce55" class="anchor"></a>

```cpp
void deepSleep()
```

Puts the device into a deep sleep mode. The device consumes the least power in this mode but will reset when it wakes up effectively running the setup() function again.
<hr />

### `enableWakeupFromPin` <a id="class_low_power_1a9ee83fbc2b670ac9490cd179b2e08138" class="anchor"></a>

```cpp
bool enableWakeupFromPin(uint8_t pin, PinStatus direction)
```

Enables wake-up of the device from a specified pin (A0, A1, A2, A3, A4, A5, D4, D7 ) 
#### Parameters
* `pin` The pin number used for waking up the device. 

* `direction` The direction of the interrupt that will wake up the device. (RISING, FALLING, CHANGE)
<hr />

### `setWakeUpAlarm` <a id="class_low_power_1ae4bf7b3a270d1a54a00a527191031334" class="anchor"></a>

```cpp
bool setWakeUpAlarm(RTCTime alarmTime, void(*)() callbackFunction, RTClock * rtc)
```

Enables wake-up of the device based on the Real-Time Clock (RTC). The device will wake up at the specified time. 
#### Parameters
* `alarmTime` The time at which the device will wake up. 

* `callbackFunction` The function to be called when the device wakes up. Specifying it only makes sense when used with [sleep()](#class_low_power_1a351563f87f8d1f118e9d77201fd59a55) and not [deepSleep()](#class_low_power_1ada3410c10d04970e0898eb5f7f36ce55) as the device will reset when it wakes up from deep sleep. 

* `rtc` The Real-Time Clock (RTC) to be used for setting the wake-up alarm. If not specified, the default RTC will be used. 

#### Returns
True if the wake-up alarm was set successfully, false otherwise.
<hr />

### `setWakeUpAlarm` <a id="class_low_power_1add42684056e6bc5573fa500fc581b9b8" class="anchor"></a>

```cpp
bool setWakeUpAlarm(uint8_t hours, uint8_t minutes, uint8_t seconds, void(*)() callbackFunction, RTClock * rtc)
```

Enables wake-up of the device based on the Real-Time Clock (RTC). The device will wake up at the specified time. 
#### Parameters
* `hours` The hour at which the device will wake up. 

* `minutes` The minute at which the device will wake up. 

* `seconds` The second at which the device will wake up. 

* `callbackFunction` The function to be called when the device wakes up. Specifying it only makes sense when used with [sleep()](#class_low_power_1a351563f87f8d1f118e9d77201fd59a55) and not [deepSleep()](#class_low_power_1ada3410c10d04970e0898eb5f7f36ce55) as the device will reset when it wakes up from deep sleep. 

* `rtc` The Real-Time Clock (RTC) to be used for setting the wake-up alarm. If not specified, the default RTC will be used. 

#### Returns
True if the wake-up alarm was set successfully, false otherwise.
<hr />

