#ifndef LOWPOWER_H
#define LOWPOWER_H

#include "r_lpm.h"
#include "Arduino.h"


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
     */
    void enableWakeupFromRTC();


private:
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