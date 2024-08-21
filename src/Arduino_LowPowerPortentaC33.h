#ifndef LOWPOWER_H
#define LOWPOWER_H

#include "r_lpm.h"
#include "Arduino.h"
#include "RTC.h"


/**
 * @class LowPower
 * @brief Provides functions for managing power and implementing sleep modes on an Arduino device.
 *
 * This class allows the user to put the device into different low-power states and configure
 * wake-up sources such as external pins or timers.
 */
class LowPower {
public:
    /**
     * Constructor for LowPower class.
     * Initializes the low power and sleep configurations.
     */
    LowPower();

    /**
     * Puts the device into a standard sleep mode.
     * This sleep mode consumes less power than the active mode but more than the deep sleep mode.
     * The device will resume execution from the point where it entered the sleep mode.
     */
    void sleep();

    /**
     * Puts the device into a deep sleep mode.
     * The device consumes the least power in this mode but will reset when it wakes up effectively running the setup() function again.
     */
    void deepSleep();

    /**
     * Enables wake-up of the device from a specified pin (A0, A1, A2, A3, A4, A5, D4, D7 )
     * @param pin The pin number used for waking up the device.
     * @param direction The direction of the interrupt that will wake up the device. (RISING, FALLING, CHANGE)
     */
    bool enableWakeupFromPin(uint8_t pin, PinStatus direction);

    /**
     * Enables wake-up of the device based on the Real-Time Clock (RTC).
     * The device will wake up at the specified time.
     * @param alarmTime The time at which the device will wake up.
     * @param callbackFunction The function to be called when the device wakes up.
     * Specifying it only makes sense when used with sleep() and not deepSleep() as the device will reset when it wakes up from deep sleep.
     * @param rtc The Real-Time Clock (RTC) to be used for setting the wake-up alarm.
     * If not specified, the default RTC will be used.
     * @return True if the wake-up alarm was set successfully, false otherwise.
    */
    bool setWakeUpAlarm(RTCTime alarmTime, void (* const callbackFunction)() = nullptr, RTClock * rtc = &RTC);
    
    /**
     * Enables wake-up of the device based on the Real-Time Clock (RTC).
     * The device will wake up at the specified time.
     * @param hours The hour at which the device will wake up.
     * @param minutes The minute at which the device will wake up.
     * @param seconds The second at which the device will wake up.
     * @param callbackFunction The function to be called when the device wakes up.
     * Specifying it only makes sense when used with sleep() and not deepSleep() as the device will reset when it wakes up from deep sleep.
     * @param rtc The Real-Time Clock (RTC) to be used for setting the wake-up alarm.
     * If not specified, the default RTC will be used.
     * @return True if the wake-up alarm was set successfully, false otherwise.
     */
    bool setWakeUpAlarm(uint8_t hours, uint8_t minutes, uint8_t seconds, void (* const callbackFunction)() = nullptr, RTClock * rtc = &RTC);

private:
    /**
     * Enables wake-up of the device based on the Real-Time Clock (RTC).
     */
    void enableWakeupFromRTC();

    /**
     * Retrieves the IRQ channel associated with a standby wake-up source for a given pin.
     * @param pin The pin number.
     * @return The IRQ channel number.
     */
    int getStandbyWakeIRQChannelForPin(uint8_t pin);

    /**
     * Retrieves the IRQ channel associated with a deep sleep wake-up source for a given pin.
     * @param pin The pin number.
     * @return The IRQ channel number.
     */
    int getDeepSleepWakeIRQChannelForPin(uint8_t pin);


    // Member variables
    uint16_t deepSleepWakeupSource = 0;
    uint16_t deepSleepWakeupEdge = 0;
    uint16_t standbyWakeupSource = 0;

    lpm_cfg_t RenesasLowPowerConfig;
    lpm_instance_ctrl_t RenesasLowPowerControlBlock;
};

#endif // LOWPOWER_H