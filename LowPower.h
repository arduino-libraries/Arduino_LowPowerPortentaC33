#ifndef LOWPOWER_H
#define LOWPOWER_H

#include "r_lpm.h"
#include "Arduino.h"
#include "FspTimer.h"

#define MAX_TIMER_COUNT 65536
#define CLOCK_FREQ_HZ 48000000 // 48 MHz

constexpr auto AUTOMATIC_SLEEP_LOW_POWER_MODE = LPM_MODE_STANDBY_SNOOZE;
constexpr auto AUTOMATIC_SLEEP_POWER_SUPPLY_STATE = LPM_POWER_SUPPLY_DEEPCUT0;
constexpr auto AUTOMATIC_SLEEP_OUTPUT_PORT_ENABLE = LPM_OUTPUT_PORT_ENABLE_RETAIN;
constexpr auto AUTOMATIC_SLEEP_IO_PORT_STATE = LPM_IO_PORT_NO_CHANGE;
constexpr auto AUTOMATIC_SLEEP_DTC_STATE_IN_SNOOZE = LPM_SNOOZE_DTC_DISABLE;
constexpr auto AUTOMATIC_SLEEP_WAKEUP_SOURCE = LPM_STANDBY_WAKE_SOURCE_IRQ15;
constexpr auto AUTOMATIC_SLEEP_DEEP_WAKEUP_SOURCE = LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ12_RISING;
constexpr auto AUTOMATIC_SLEEP_TIMER_PULSE = 1;
constexpr auto AUTOMATIC_SLEEP_TIMER_CHANNEL = 5;
constexpr auto AUTOMATIC_SLEEP_TIMER_IRQ_CHANNEL = 12;

constexpr auto VOLUNTARY_SLEEP_LOW_POWER_MODE = LPM_MODE_DEEP;
constexpr auto VOLUNTARY_SLEEP_POWER_SUPPLY_STATE = LPM_POWER_SUPPLY_DEEPCUT0;
constexpr auto VOLUNTARY_SLEEP_OUTPUT_PORT_ENABLE = LPM_OUTPUT_PORT_ENABLE_HIGH_IMPEDANCE;
constexpr auto VOLUNTARY_SLEEP_IO_PORT_STATE = LPM_IO_PORT_RESET;
constexpr auto VOLUNTARY_SLEEP_DTC_STATE_IN_SNOOZE = LPM_SNOOZE_DTC_DISABLE;



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
     * The device can be woken up by predefined interrupts or conditions.
     */
    void sleep();

    /**
     * Puts the device into a deep sleep mode.
     * The device consumes the least power in this mode but requires specific conditions to wake up.
     */
    void deepSleep();

    /**
     * Enables wake-up of the device from a specified pin.
     * @param pin The pin number used for waking up the device.
     */
    void enableWakeupFromPin(uint8_t pin);

    /**
     * Enables wake-up of the device based on the Real-Time Clock (RTC).
     */
    void enableWakeupFromRTC();

 


    /**
     * Custom delay function that puts the device into a low-power state while waiting.
     * @param ms Number of milliseconds to delay.
     */
    void delay(long ms);

    /**
     * Static callback function for the timer.
     * @param p_args Pointer to the timer callback arguments.
     */
    static void timerCallback(timer_callback_args_t *p_args);

    /**
     * Flag indicating whether a timer event has occurred.
     */
    static volatile bool timer_event;

private:
   /**
     * Configures and enables a timer to allow the device to sleep until the timer expires.
     */
    void enableSleepInDelay();

    /**
     * Changes the sleep mode of the device.
     * @param automatic True for automatic sleep mode, false for voluntary sleep mode.
     * @return True if the operation was successful.
     */
    bool changeSleepMode(bool automatic);

    /**
     * Sets a timer for a specified period.
     * @param period The period in milliseconds for the timer.
     */
    void setTimerForPeriod(uint32_t period);

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
    uint16_t deepSleepWakeupSource;
    uint16_t standbyWakeupSource;
    
    lpm_instance_ctrl_t lpm_ctrl_voluntary_sleep;
    lpm_instance_ctrl_t lpm_ctrl_automatic_sleep;
    lpm_cfg_t lpm_cfg_voluntary_sleep;
    lpm_cfg_t lpm_cfg_automatic_sleep;
    FspTimer *automaticSleepTimer;

    static unsigned long cyclesDone;
    static uint32_t timerPeriodTicks, numOverflows;
    bool sleeping = false;
    bool initialized = false;
    bool automaticSleep = false;
    bool voluntarySleep = false;
};

#endif // LOWPOWER_H