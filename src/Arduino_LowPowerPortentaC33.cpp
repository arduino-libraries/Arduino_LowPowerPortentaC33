#include "Arduino_LowPowerPortentaC33.h"

LowPower::LowPower(){
    RenesasLowPowerConfig.dtc_state_in_snooze = LPM_SNOOZE_DTC_DISABLE;
    RenesasLowPowerConfig.power_supply_state = LPM_POWER_SUPPLY_DEEPCUT0;
    RenesasLowPowerConfig.output_port_enable = LPM_OUTPUT_PORT_ENABLE_HIGH_IMPEDANCE;
    RenesasLowPowerConfig.io_port_state = LPM_IO_PORT_RESET;
    RenesasLowPowerConfig.low_power_mode = LPM_MODE_DEEP;
    RenesasLowPowerConfig.standby_wake_sources = LPM_STANDBY_WAKE_SOURCE_IRQ15;
    RenesasLowPowerConfig.deep_standby_cancel_source = LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ15;
  
}

void LowPower::sleep(){
    RenesasLowPowerConfig.low_power_mode = LPM_MODE_STANDBY;
    R_LPM_Open(&RenesasLowPowerControlBlock, &RenesasLowPowerConfig);
    R_LPM_LowPowerModeEnter(&RenesasLowPowerControlBlock);
}

void LowPower::deepSleep(){
    RenesasLowPowerConfig.low_power_mode = LPM_MODE_DEEP;
    R_LPM_Open(&RenesasLowPowerControlBlock, &RenesasLowPowerConfig);
    R_LPM_LowPowerModeEnter(&RenesasLowPowerControlBlock);
}

void LowPower::enableWakeupFromRTC(){
    RenesasLowPowerConfig.deep_standby_cancel_source = LPM_DEEP_STANDBY_CANCEL_SOURCE_RTC_ALARM;
    RenesasLowPowerConfig.standby_wake_sources = LPM_STANDBY_WAKE_SOURCE_RTCALM;
}
 
bool LowPower::enableWakeupFromPin(uint8_t pin, PinStatus direction){
    attachInterrupt(digitalPinToInterrupt(pin), nullptr, direction);

    if (getStandbyWakeIRQChannelForPin(pin) == -1)
        return false;
 
    if (getDeepSleepWakeIRQChannelForPin(pin) == -1)
        return false;


    if(deepSleepWakeupSource == 0)
        deepSleepWakeupSource = getDeepSleepWakeIRQChannelForPin(pin);
    else
        deepSleepWakeupSource = deepSleepWakeupSource | getDeepSleepWakeIRQChannelForPin(pin);


    if(standbyWakeupSource == 0)
        standbyWakeupSource = getStandbyWakeIRQChannelForPin(pin);
    else
        standbyWakeupSource = standbyWakeupSource | getStandbyWakeIRQChannelForPin(pin);

    RenesasLowPowerConfig.deep_standby_cancel_source = deepSleepWakeupSource;
    RenesasLowPowerConfig.standby_wake_sources = standbyWakeupSource;

    return true;
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
        return -1;
    }
}


// direction of the interrupt is rising ?
int LowPower::getDeepSleepWakeIRQChannelForPin(uint8_t pin){
  switch (pin)
        {
            case A0: return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ11;
            case A1: return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ10;
            case A2: return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ9;
            case A3: return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ8;
            case A4: return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ7;
            case A5: return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ13;
            case 4:  return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ5;
            case 7:  return LPM_DEEP_STANDBY_CANCEL_SOURCE_IRQ4;
        default:
            return -1;
        }
}