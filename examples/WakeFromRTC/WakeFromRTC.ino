

#include "RTC.h"
#include "LowPower.h"

LowPower lowPower;


RTCTime initial_time(1, Month::JANUARY, 2000, 12, 10, 00, DayOfWeek::TUESDAY, SaveLight::SAVING_TIME_ACTIVE);


void alarmCallback()
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
    lowPower.deepSleep();
}


void setup(){
    lowPower = LowPower();
    lowPower.enableWakeupFromRTC(&initialTime, &callback);
    lowPower.enableWakeupFromRTC();

    pinMode(LED_BUILTIN, OUTPUT);

    RTC.begin();

    if (!RTC.isRunning()) RTC.setTime(initial_time);

    // Trigger the alarm every time the seconds are zero
    RTCTime alarmTime;
    alarmTime.setSecond(0);

    // Make sure to only match on the seconds in this example - not on any other parts of the date/time
    AlarmMatch matchTime;
    matchTime.addMatchSecond();

    //sets the alarm callback
    RTC.setAlarmCallback(alarmCallback, alarmTime, matchTime);
  
}

void loop(){
    lowPower.deepSleep();
}
