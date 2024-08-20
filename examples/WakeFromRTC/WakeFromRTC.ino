/*
********************************************************************************
*
* This example demonstrates how to use the RTC to wake up the
* Portenta C33 from deep sleep. The device will go to sleep for 5 seconds and
* then wake up. The built-in LED will blink every second.
*
* The example also demonstrates how to use the PF1550 PMIC to turn off the peripherals
* before going to sleep and turn them back on after waking up.
* uncomment #define TURN_PERIPHERALS_OFF to enable this feature.
*
* INSTRUCTIONS:
* - Make sure you are running the latest version of the Renesas Core
* - Select the Portenta C33 board from the Tools menu
* - Select the Portenta C33 USB port from the Tools menu
* - Upload the code to your Portenta C33
*  
* Initial author: C. Dragomir
*/

// #define TURN_PERIPHERALS_OFF // Uncomment this line to turn off the peripherals before going to sleep

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

/**
 * Initializes the Real-Time Clock (RTC) and sets the initial time if the RTC is not running.
 * This initial time is a dummy time. You can obtain the current time from an NTP server and set it here.
 */
void initializeRealTimeClock(){
    RTC.begin();

    if (!RTC.isRunning()) {
        RTC.setTime(initialTime);
    }
}

void goToSleep(){
    #ifdef TURN_PERIPHERALS_OFF
        // Turn peripherals off before going to sleep 
        // LED turns off automatically as part of this process
        turnPeripheralsOff();
    #else
        // Turn off the LED to indicate the device is going to sleep
        digitalWrite(LED_BUILTIN, HIGH);
    #endif
    lowPower.deepSleep();
}

void setup(){
    #ifdef TURN_PERIPHERALS_OFF
        PMIC.begin();
        turnPeripheralsOn();
    #endif

    initializeRealTimeClock();

    pinMode(LED_BUILTIN, OUTPUT); // Set the LED pin as an output    
    pinMode(LEDR, OUTPUT); // Use the red LED to indicate errors
    digitalWrite(LEDR, HIGH); // Turn off the red LED
    digitalWrite(LED_BUILTIN, LOW); // Turn on the LED

    delay(5000); // lets the user see the led for 5 seconds
    
    // The device will go to sleep after 5 seconds and wake up after 5 seconds to light up the LED
    // effectivelly creating the same visual effect as the blink sketch.
    if(!lowPower.setWakeUpAlarm(0, 0, 5)){
         // Blink the red LED indefinitely to indicate an error
        while(true){
            digitalWrite(LEDR, HIGH);
            delay(500);
            digitalWrite(LEDR, LOW);
            delay(500);
        }
    }

    goToSleep();
}

void loop(){}
