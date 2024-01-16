
#include "LowPower.h"

volatile bool LowPower::timer_event = false;
long unsigned int LowPower::cyclesDone = 0;
uint32_t LowPower::timerPeriodTicks = 0;
uint32_t LowPower::numOverflows = 0;




void LowPower::setTimerForPeriod(uint32_t desiredPeriodMs) {
    // get the clock frequency from FSP
    const uint32_t clock_freq_Hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB);
    uint32_t effectiveClock = clock_freq_Hz / ((1 << TIMER_SOURCE_DIV_8));

    // calculate the number of ticks that are needed 
    uint64_t totalTicksNeeded = (uint64_t)effectiveClock * desiredPeriodMs / 1000;
    LowPower::timerPeriodTicks = (totalTicksNeeded <= MAX_TIMER_COUNT) ? totalTicksNeeded : MAX_TIMER_COUNT;
    LowPower::numOverflows = totalTicksNeeded / MAX_TIMER_COUNT;
    if (totalTicksNeeded % MAX_TIMER_COUNT != 0) {
        LowPower::numOverflows += 1; // Adjust for any remainder
    }
}

void LowPower::timerCallback(timer_callback_args_t * p_args) {
    if (TIMER_EVENT_CYCLE_END == p_args->event) {
        LowPower::cyclesDone++;
        LowPower::timer_event = true;
    }
}

// initializes the timer for automatic sleep
void LowPower::enableSleepInDelay(){
    setTimerForPeriod(0);

    automaticSleepTimer = new FspTimer();
    automaticSleepTimer -> begin(/* mode */ TIMER_MODE_PERIODIC,
                  /* type */ AGT_TIMER,
                  /* channel */ AUTOMATIC_SLEEP_TIMER_CHANNEL,
                  LowPower::timerPeriodTicks,
                  /* pulse */ AUTOMATIC_SLEEP_TIMER_PULSE,
                  TIMER_SOURCE_DIV_8,
                  timerCallback);

    automaticSleepTimer -> setup_overflow_irq(AUTOMATIC_SLEEP_TIMER_IRQ_CHANNEL);
    automaticSleepTimer -> open();
    automaticSleepTimer -> stop();

    // changes to automatic sleep mode
    changeSleepMode(true);
}



void LowPower::delay(long ms) {
    enableSleepInDelay();
    if (automaticSleepTimer -> is_opened()) {
        if (!sleeping) {
            // initialising sleep for the given period
            setTimerForPeriod(ms);
            automaticSleepTimer -> set_period(LowPower::timerPeriodTicks);
            automaticSleepTimer -> start();
            sleeping = true;
            R_LPM_LowPowerModeEnter(&lpm_ctrl_automatic_sleep);
        } else {
            if (LowPower::timer_event) {
                LowPower::timer_event = false;
                // if the number of overflows has been reached, stop the timer and exit sleep mode
                if (LowPower::numOverflows == cyclesDone) {
                    sleeping = false;
                    cyclesDone = 0;
                    automaticSleepTimer -> stop();
                    automaticSleepTimer -> reset();
                // otherwise, re-enter sleep mode
                } else {
                    sleeping = true;
                    R_LPM_LowPowerModeEnter(&lpm_ctrl_automatic_sleep);
                }
            } else {
                // wait for the timer event to occur
                while (!LowPower::timer_event);
            }
        }
    }
}


bool LowPower::changeSleepMode(bool automatic) {
    // Check if the mode change is necessary
    if (initialized) {
        if ((automatic && automaticSleep) || (!automatic && voluntarySleep)) {
            // Already in the desired mode
            return true;
        }

        // Close the current mode
        if (automaticSleep) {
            R_LPM_Close(&lpm_ctrl_automatic_sleep);
        } else {
            R_LPM_Close(&lpm_ctrl_voluntary_sleep);
        }
    }

    // Open the new mode
    if (automatic) {
        R_LPM_Open(&lpm_ctrl_automatic_sleep, &lpm_cfg_automatic_sleep);
        automaticSleep = true;
        voluntarySleep = false;
    } else {
        R_LPM_Open(&lpm_ctrl_voluntary_sleep, &lpm_cfg_voluntary_sleep);
        automaticSleep = false;
        voluntarySleep = true;
    }

    initialized = true;
    return true;
}



LowPower::LowPower(){
    


    lpm_cfg_automatic_sleep.low_power_mode = LPM_MODE_STANDBY_SNOOZE; //  LPM_MODE_SLEEP  LPM_MODE_STANDBY    LPM_MODE_STANDBY_SNOOZE    LPM_MODE_DEEP
    lpm_cfg_automatic_sleep.standby_wake_sources = LPM_STANDBY_WAKE_SOURCE_IRQ15;
    lpm_cfg_automatic_sleep.dtc_state_in_snooze = LPM_SNOOZE_DTC_DISABLE; // LPM_SNOOZE_DTC_ENABLE LPM_SNOOZE_DTC_DISABLE
    lpm_cfg_automatic_sleep.power_supply_state = LPM_POWER_SUPPLY_DEEPCUT0; // LPM_POWER_SUPPLY_DEEPCUT0, LPM_POWER_SUPPLY_DEEPCUT1, LPM_POWER_SUPPLY_DEEPCUT3
    lpm_cfg_automatic_sleep.output_port_enable = LPM_OUTPUT_PORT_ENABLE_HIGH_IMPEDANCE; // LPM_OUTPUT_PORT_ENABLE_RETAIN LPM_OUTPUT_PORT_ENABLE_HIGH_IMPEDANCE
    lpm_cfg_automatic_sleep.io_port_state = LPM_IO_PORT_RESET;  // LPM_IO_PORT_NO_CHANGE LPM_IO_PORT_RESET
    lpm_cfg_automatic_sleep.deep_standby_cancel_source = LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ15_RISING;

    lpm_cfg_voluntary_sleep.dtc_state_in_snooze = VOLUNTARY_SLEEP_DTC_STATE_IN_SNOOZE;
    lpm_cfg_voluntary_sleep.power_supply_state = VOLUNTARY_SLEEP_POWER_SUPPLY_STATE;
    lpm_cfg_voluntary_sleep.output_port_enable = VOLUNTARY_SLEEP_OUTPUT_PORT_ENABLE;
    lpm_cfg_voluntary_sleep.io_port_state = VOLUNTARY_SLEEP_IO_PORT_STATE;
    lpm_cfg_voluntary_sleep.low_power_mode = VOLUNTARY_SLEEP_LOW_POWER_MODE;

}

void LowPower::sleep(){
    changeSleepMode(false);
    lpm_cfg_voluntary_sleep.low_power_mode = LPM_MODE_STANDBY;
    R_LPM_LowPowerModeEnter(&lpm_ctrl_voluntary_sleep);
}

void LowPower::deepSleep(){
    changeSleepMode(false);
    lpm_cfg_voluntary_sleep.low_power_mode = LPM_MODE_DEEP;
    R_LPM_LowPowerModeEnter(&lpm_ctrl_voluntary_sleep);
}

void LowPower::enableWakeupFromRTC(){
    lpm_cfg_voluntary_sleep.deep_standby_cancel_source = LPM_DEEP_STANDBY_CANCEL_SOURCE_RTC_ALARM;
    lpm_cfg_voluntary_sleep.standby_wake_sources =  LPM_STANDBY_WAKE_SOURCE_RTCALM;
}

void LowPower::enableWakeupFromPin(uint8_t pin){
   // changeSleepMode(false);
    attachInterrupt(digitalPinToInterrupt(pin), nullptr, RISING);
    deepSleepWakeupSource = getDeepSleepWakeIRQChannelForPin(pin);
    standbyWakeupSource =  getStandbyWakeIRQChannelForPin(pin);
    lpm_cfg_voluntary_sleep.deep_standby_cancel_source = deepSleepWakeupSource;
    lpm_cfg_voluntary_sleep.standby_wake_sources = standbyWakeupSource;
}

int LowPower::getStandbyWakeIRQChannelForPin(uint8_t pin){
    switch (pin)
    {
        case A0: return LPM_STANDBY_WAKE_SOURCE_IRQ11;
        case A1: return LPM_STANDBY_WAKE_SOURCE_IRQ10;
        case A2: return LPM_STANDBY_WAKE_SOURCE_IRQ9;
        case A3: return LPM_STANDBY_WAKE_SOURCE_IRQ8;
        case A4: return LPM_STANDBY_WAKE_SOURCE_IRQ7;
        case A5: return LPM_STANDBY_WAKE_SOURCE_IRQ13;
        case 4: return LPM_STANDBY_WAKE_SOURCE_IRQ5;
        case 7: return LPM_STANDBY_WAKE_SOURCE_IRQ4;
    default:
        break;
    }
}

int LowPower::getDeepSleepWakeIRQChannelForPin(uint8_t pin){
    switch (pin)
    {
        case A0: return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ11_RISING;
        case A1: return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ10_RISING;
        case A2: return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ9_RISING;
        case A3: return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ8_RISING;
        case A4: return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ7_RISING;
        case A5: return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ13_RISING;
        case 4:  return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ5_RISING;
        case 7:  return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ4_RISING;
    default:
        break;
    }
}