/*
********************************************************************************
*                          WakeFromGPIO.ino
*
* This example demonstrates how to wake up the Portenta C33 from deep sleep using a GPIO pin.
* The pin can be configured to wake up the device on a rising or falling edge, but not all pins are supported.
* Please check the README.md file for more information about the supported pins.
*
* The example uses two buttons connected to pin 0 and A3:
*     * The device will go to sleep when the button connected to pin 0 is pressed.
*     * The device will wake up when the button connected to pin A3 is pressed.
* 
* The example also demonstrates how to use the PF1550 PMIC to turn off the peripherals
* before going to sleep and turn them back on after waking up.
* uncomment #define TURN_PERIPHERALS_OFF on line 28 to enable this feature.
*
* When the device is not sleeping it will blink the built-in LED every 100ms. 
* 
* INSTRUCTIONS:
* - Make sure you are running the latest version of the Renesas Core
* - Select the Portenta C33 board from the Tools menu
* - Select the Portenta C33 USB port from the Tools menu
* - Upload the code to your Portenta C33
* - Connect a button to pin 0 and with a pull-up resistor to 3.3V
* - Connect a button to pin A3 and with a pull-up resistor to 3.3V 
* (If you need information about how to wire the buttons check this link: https://docs.arduino.cc/built-in-examples/digital/Button/)
* 
* Original author: C. Dragomir (http://arduino.cc)
*/

#include "Arduino_LowPowerPortentaC33.h"

// #define TURN_PERIPHERALS_OFF
#define SLEEP_PIN 0
#define WAKE_PIN A3

LowPower lowPower;

#ifdef TURN_PERIPHERALS_OFF
    #include "Arduino_PMIC.h"
#endif

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

void goToSleep(){
    turnPeripheralsOff();
    lowPower.deepSleep(); 
}

void setup(){
    lowPower = LowPower();
    attachInterrupt(digitalPinToInterrupt(SLEEP_PIN), goToSleep, RISING);
    lowPower.enableWakeupFromPin(WAKE_PIN, RISING);
    pinMode(LED_BUILTIN, OUTPUT);

    #ifdef TURN_PERIPHERALS_OFF
        PMIC.begin();
        turnPeripheralsOn();
    #endif
}

void loop(){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}




