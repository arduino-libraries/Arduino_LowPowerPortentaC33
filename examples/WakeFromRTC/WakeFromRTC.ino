

#include "RTC.h"
#include "Arduino_LowPowerPortentaC33.h"

LowPower lowPower;
RTCTime initialTime(1, Month::JANUARY, 2000, 12, 10, 00, DayOfWeek::TUESDAY, SaveLight::SAVING_TIME_ACTIVE);


void alarmCallback()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
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
    Serial.begin(9600);
    //while(!Serial);

    lowPower = LowPower();
    lowPower.enableWakeupFromRTC();

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    RTC.begin();

    if (!RTC.isRunning()) {
        RTC.setTime(initialTime);
        sleepFor(0, 0, 1);
    }
  
}

void loop(){
    lowPower.deepSleep();
}
