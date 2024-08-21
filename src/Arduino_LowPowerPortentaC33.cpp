#include "Arduino_LowPowerPortentaC33.h"
#include "r_lpm.h"

LowPower::LowPower(){
    RenesasLowPowerConfig.dtc_state_in_snooze = LPM_SNOOZE_DTC_DISABLE;
    RenesasLowPowerConfig.power_supply_state = LPM_POWER_SUPPLY_DEEPCUT3 ;
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

bool LowPower::setWakeUpAlarm(RTCTime alarmTime, void (* const callbackFunction)(), RTClock * rtc){
    this->enableWakeupFromRTC();

    if(!rtc->isRunning()){
        return false;
    }

    RTCTime currentTime;
    if (!rtc->getTime(currentTime)) {
        return false; // Failed to get current time
    }

    // Configure the alarm match criteria
    AlarmMatch match;
    match.addMatchSecond(); // Trigger the alarm when the seconds match
    match.addMatchMinute(); // Trigger the alarm when the minutes match
    match.addMatchHour();   // Trigger the alarm when the hours match
    match.addMatchDay();    // Trigger the alarm when the days match
    match.addMatchMonth();  // Trigger the alarm when the months match
    match.addMatchYear();   // Trigger the alarm when the years match

    if (callbackFunction) {
        return rtc->setAlarmCallback(callbackFunction, alarmTime, match);
    } else {
        return rtc->setAlarm(alarmTime, match);
    }
}

bool LowPower::setWakeUpAlarm(uint8_t hours, uint8_t minutes, uint8_t seconds, void (* const callbackFunction)(), RTClock * rtc){

    if(!rtc->isRunning()){
        return false;
    }

    // Get the current time from the RTC
    RTCTime currentTime;
    if (!rtc->getTime(currentTime)) {
        return false; // Failed to get current time
    }

    // Convert current time to UNIX timestamp and add the desired interval
    time_t currentTimestamp = currentTime.getUnixTime();
    currentTimestamp += hours * 3600 + minutes * 60 + seconds;

    // Convert back to RTCTime
    RTCTime alarmTime(currentTimestamp);

    return this->setWakeUpAlarm(alarmTime, callbackFunction, rtc);
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