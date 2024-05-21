

#include "RTC.h"
#include "Arduino_LowPowerPortentaC33.h"
#include "Arduino_PMIC.h"



LowPower lowPower;
RTCTime initialTime(1, Month::JANUARY, 2000, 12, 10, 00, DayOfWeek::TUESDAY, SaveLight::SAVING_TIME_ACTIVE);


void turnOffPeripherals(){
    //stopETHClock();
        // Turn off the external power regulator
    PMIC.getControl() -> turnSw2Off(Sw2Mode::Normal);
    PMIC.getControl() -> turnSw2Off(Sw2Mode::Sleep);
    PMIC.getControl() -> turnSw2Off(Sw2Mode::Standby);

    
        // Turn off the Ethernet PHY, SDRAM, and USB PHY
    PMIC.getControl() -> turnSw1Off(Sw1Mode::Normal);
    PMIC.getControl() -> turnSw1Off(Sw1Mode::Sleep);
    PMIC.getControl() -> turnSw1Off(Sw1Mode::Standby);


    
    
    PMIC.getControl() -> turnLDO1Off(Ldo1Mode::Normal);
    PMIC.getControl() -> turnLDO1Off(Ldo1Mode::Sleep);
    PMIC.getControl() -> turnLDO1Off(Ldo1Mode::Standby);


    // Turn off misc onboard chips
    PMIC.getControl() -> turnLDO2Off(Ldo2Mode::Normal);
    PMIC.getControl() -> turnLDO2Off(Ldo2Mode::Sleep);
    PMIC.getControl() -> turnLDO2Off(Ldo2Mode::Standby);
    

    // Turn off the 1.2V for Ethernet
    PMIC.getControl() -> turnLDO3Off(Ldo3Mode::Normal);
    PMIC.getControl() -> turnLDO3Off(Ldo3Mode::Sleep);
    PMIC.getControl() -> turnLDO3Off(Ldo3Mode::Standby);
        delay(1);

}

void turnOnPeripherals(){
       // startETHClock();
 PMIC.getControl() -> turnLDO1On(Ldo1Mode::Normal);
    PMIC.getControl() -> turnLDO1On(Ldo1Mode::Sleep);
    PMIC.getControl() -> turnLDO1On(Ldo1Mode::Standby);

    // Turn off misc onboard chips
    PMIC.getControl() -> turnLDO2On(Ldo2Mode::Normal);
    PMIC.getControl() -> turnLDO2On(Ldo2Mode::Sleep);
    PMIC.getControl() -> turnLDO2On(Ldo2Mode::Standby);

    // Turn off the 1.2V for Ethernet
    PMIC.getControl() -> turnLDO3On(Ldo3Mode::Normal);
    PMIC.getControl() -> turnLDO3On(Ldo3Mode::Sleep);
    PMIC.getControl() -> turnLDO3On(Ldo3Mode::Standby);

    // Turn off the Ethernet PHY, SDRAM, and USB PHY
    PMIC.getControl() -> turnSw1On(Sw1Mode::Normal);
    PMIC.getControl() -> turnSw1On(Sw1Mode::Sleep);
    PMIC.getControl() -> turnSw1On(Sw1Mode::Standby);

    // Turn off the external power regulator
    PMIC.getControl() -> turnSw2On(Sw2Mode::Normal);
    PMIC.getControl() -> turnSw2On(Sw2Mode::Sleep);
    PMIC.getControl() -> turnSw2On(Sw2Mode::Standby);
}

void alarmCallback()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  turnOffPeripherals();
  lowPower.deepSleep();
}


bool sleepFor(int hours, int minutes, int seconds){
{
    
        RTCTime currentTime;
        if (!RTC.getTime(currentTime)) {
            Serial.println("Failed to get current time"); 
            return false; // Failed to get current time
        }
        Serial.println(currentTime.toString());

        // Convert current time to UNIX timestamp and add the desired interval
        time_t currentTimestamp = currentTime.getUnixTime();
        currentTimestamp += hours * 3600 + minutes * 60 + seconds;
        // Convert back to RTCTime
        RTCTime alarmTime(currentTimestamp);
        // Configure the alarm match criteria
        AlarmMatch match;
        match.addMatchSecond(); // Trigger the alarm when the seconds match
        match.addMatchMinute(); // Trigger the alarm when the minutes match
        match.addMatchHour();   // Trigger the alarm when the hours match
        // Set the alarm callback (assuming you have a callback function named alarmCallback)
        if (!RTC.setAlarmCallback(alarmCallback, alarmTime, match)) {
            Serial.println("Failed to set the alarm");
            return false; // Failed to set the alarm
        }

        // Enable the alarm
        Serial.println("Enabling the alarm");
        return true;
    }
}


void setup(){
 
    PMIC.begin();

    turnOnPeripherals();
    Serial.begin(9600);


    lowPower = LowPower();
    lowPower.enableWakeupFromRTC();

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    RTC.begin();

    if (!RTC.isRunning()) {
        RTC.setTime(initialTime);
        sleepFor(0, 0, 1);
    }
    
    turnOffPeripherals();
    lowPower.deepSleep();
}

void loop(){
    
}
