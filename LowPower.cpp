#include "LowPower.h"

void LowPower::delay(long ms){

}

LowPower::LowPower(){
    lpm_voluntary_sleep_cfg.dtc_state_in_snooze = LPM_SNOOZE_DTC_DISABLE; // LPM_SNOOZE_DTC_ENABLE LPM_SNOOZE_DTC_DISABLE
    lpm_voluntary_sleep_cfg.power_supply_state = LPM_POWER_SUPPLY_DEEPCUT3; // LPM_POWER_SUPPLY_DEEPCUT0, LPM_POWER_SUPPLY_DEEPCUT1, LPM_POWER_SUPPLY_DEEPCUT3
    lpm_voluntary_sleep_cfg.output_port_enable = LPM_OUTPUT_PORT_ENABLE_HIGH_IMPEDANCE; // LPM_OUTPUT_PORT_ENABLE_RETAIN LPM_OUTPUT_PORT_ENABLE_HIGH_IMPEDANCE
    lpm_voluntary_sleep_cfg.io_port_state = LPM_IO_PORT_RESET;  // LPM_IO_PORT_NO_CHANGE LPM_IO_PORT_RESET

}

void LowPower::sleep(){
    lpm_voluntary_sleep_cfg.low_power_mode = LPM_MODE_STANDBY;
    R_LPM_Open(&lpm_voluntary_sleep_ctrl, &lpm_voluntary_sleep_cfg);
    R_LPM_LowPowerModeEnter(&lpm_voluntary_sleep_ctrl);
}

void LowPower::deepSleep(){
    lpm_voluntary_sleep_cfg.low_power_mode = LPM_MODE_DEEP;
    R_LPM_Open(&lpm_voluntary_sleep_ctrl, &lpm_voluntary_sleep_cfg);
    R_LPM_LowPowerModeEnter(&lpm_voluntary_sleep_ctrl);
}

void LowPower::setWakeupRTC(){
    lpm_voluntary_sleep_cfg.deep_standby_cancel_source = LPM_DEEP_STANDBY_CANCEL_SOURCE_RTC_ALARM;
    lpm_voluntary_sleep_cfg.standby_wake_sources = LPM_STANDBY_WAKE_SOURCE_RTCALM ;
}

void LowPower::setWakeupPin(uint8_t pin){
    attachInterrupt(digitalPinToInterrupt(pin), nullptr, RISING);
    deepSleepWakeupSource = deepSleepWakeupSource | getDeepSleepWakeIRQChannelForPin(pin);
    standbyWakeupSource = standbyWakeupSource | getStandbyWakeIRQChannelForPin(pin);
    lpm_voluntary_sleep_cfg.deep_standby_cancel_source = deepSleepWakeupSource;
    lpm_voluntary_sleep_cfg.standby_wake_sources = standbyWakeupSource;
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