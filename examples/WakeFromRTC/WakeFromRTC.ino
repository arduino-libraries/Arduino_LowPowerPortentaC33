/*
********************************************************************************
*                          WakeFromRTC.ino
*
* This example demonstrates how to use the RTC to wake up the
* Portenta C33 from deep sleep. The device will go to sleep for 1 second and
* then wake up. The built-in LED will blink every second.
*
* The example also demonstrates how to use the PF1550 PMIC to turn off the peripherals
* before going to sleep and turn them back on after waking up.
* uncomment #define TURN_PERIPHERALS_OFF on line 24 to enable this feature.
*
* INSTRUCTIONS:
* - Make sure you are running the latest version of the Renesas Core
* - Select the Portenta C33 board from the Tools menu
* - Select the Portenta C33 USB port from the Tools menu
* - Upload the code to your Portenta C33
*  
* Original author: C. Dragomir (http://arduino.cc)

*/

// #define TURN_PERIPHERALS_OFF

#include "RTC.h"
#include "Arduino_LowPowerPortentaC33.h"

#ifdef TURN_PERIPHERALS_OFF
    #include "Arduino_PMIC.h"
#endif

LowPower lowPower;
RTCTime initialTime(1, Month::JANUARY, 2000, 12, 10, 00, DayOfWeek::TUESDAY, SaveLight::SAVING_TIME_ACTIVE);

#ifdef TURN_PERIPHERALS_OFF
    void turnPeripheralsOff(){
        PMIC.getControl() -> turnLDO1Off(Ldo1Mode::Normal); // LDO1 is used for AVCC (Analog Power)
        PMIC.getControl() -> turnLDO2Off(Ldo2Mode::Normal); // LDO2 is used for the analog voltage reference
        PMIC.getControl() -> turnLDO3Off(Ldo3Mode::Normal); // LDO3 is a 1.2V regulator used by the Ethernet chip
        PMIC.getControl() -> turnSw1Off(Sw1Mode::Normal);   // SW1 is a 3.3V regulator used by RGB Led, WIFI Chip, Secure element
        PMIC.getControl() -> turnSw2Off(Sw2Mode::Normal);   // SW2 is the power regulator used for powering external circuits (Labeled 3V3 on the board)
    }

    void turnPeripheralsOn(){
        PMIC.getControl() -> turnLDO1On(Ldo1Mode::Normal); // LDO1 is used for AVCC (Analog Power)
        PMIC.getControl() -> turnLDO2On(Ldo2Mode::Normal); // LDO2 is used for the analog voltage reference
        PMIC.getControl() -> turnLDO3On(Ldo3Mode::Normal); // LDO3 is a 1.2V regulator used by the Ethernet chip
        PMIC.getControl() -> turnSw1On(Sw1Mode::Normal);   // SW1 is a 3.3V regulator used by RGB Led, WIFI Chip, Secure element
        PMIC.getControl() -> turnSw2On(Sw2Mode::Normal);   // SW2 is the power regulator used for powering external circuits (Labeled 3V3 on the board)
    }
#endif

bool sleepFor(int hours, int minutes, int seconds)
{
    if(RTC.isRunning()){
        // Get the current time from the RTC
        RTCTime currentTime;
        if (!RTC.getTime(currentTime)) {
            return false; // Failed to get current time
        }

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

        // Set the alarm 
        if (!RTC.setAlarm(alarmTime, match)) {
            return false; // Failed to set the alarm
        }

        // Enable the alarm
        return true;
    }
}

void setup(){
    lowPower = LowPower();
    lowPower.enableWakeupFromRTC();

    #ifdef TURN_PERIPHERALS_OFF
        PMIC.begin();
        turnPeripheralsOn();
    #endif

    // Initialize the RTC
    RTC.begin();

    // Set the initial time if the RTC is not running
    if (!RTC.isRunning()) {
        RTC.setTime(initialTime);
    }

    pinMode(LED_BUILTIN, OUTPUT); // Set the LED pin as an output
    digitalWrite(LED_BUILTIN, LOW); // Turn on the LED
    delay(1000); // lets the user see the led for 1 second
    
    // The device will wake up every second and blink the LED
    // effectivelly creating the same efect as the blink sketch
    if(sleepFor(0, 0, 1)){
        // turn peripherals off before going to sleep 
        #ifdef TURN_PERIPHERALS_OFF
            turnPeripheralsOff();
        #endif
        lowPower.deepSleep();
    }
}

void loop(){
}
