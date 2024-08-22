/*
* This example demonstrates how to wake up the Portenta C33 from deep sleep using a GPIO pin.
* The pin can be configured to wake up the device on a rising or falling edge, but not all pins are supported.
* Please check the README.md file for more information about the supported pins.
*
* The example uses two buttons connected to pin D0 and A3:
*     * The device will go to sleep when the button connected to pin D0 is pressed.
*     * The device will wake up when the button connected to pin A3 is pressed.
* 
* The example also demonstrates how to use the PF1550 PMIC to turn off the peripherals
* before going to sleep and turn them back on after waking up.
* uncomment #define TURN_PERIPHERALS_OFF to enable this feature.
*
* When the device is not sleeping it will blink the built-in LED. 
* 
* INSTRUCTIONS:
* - Make sure you are running the latest version of the Portenta C33 Core
* - Select the Portenta C33 board from the Tools menu
* - Select the Portenta C33's USB port from the Tools menu
* - Upload the code to your Portenta C33
* - Connect a button to pin D0 and ground with a pull-up resistor to 3.3V
* - Connect a button to pin D4 and ground with a pull-up resistor to 3.3V 
*   
*   For maximum power saving use external pull-up resistors.
*   You will need to power them separately as the 3.3V pin on the board 
*   is turned off when the device goes to sleep and TURN_PERIPHERALS_OFF is enabled.
*   Alternatively, use pinMode(<pin>, INPUT_PULLUP) for the pins and connect the buttons to ground.
* (If you need information about how to wire the buttons check this link: https://docs.arduino.cc/built-in-examples/digital/Button/)
* 
* Initial author: C. Dragomir
*/

#include "Arduino_LowPowerPortentaC33.h"

// #define TURN_PERIPHERALS_OFF // Uncomment this line to turn off the peripherals before going to sleep
#define SLEEP_PIN D0 // Pin used to put the device to sleep
#define WAKE_PIN A3 // Pin used to wake up the device

LowPower lowPower;
bool shouldSleep = false;

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
    shouldSleep = true;
}

void setup(){
    // External pull-up resistor used for the buttons for maximum power saving
    pinMode(SLEEP_PIN, INPUT);
    pinMode(WAKE_PIN, INPUT);

    // Register the callback function to put the device to sleep when the button is pressed
    attachInterrupt(digitalPinToInterrupt(SLEEP_PIN), goToSleep, FALLING);
    lowPower.enableWakeupFromPin(WAKE_PIN, FALLING);
    
    pinMode(LED_BUILTIN, OUTPUT);

    #ifdef TURN_PERIPHERALS_OFF
        PMIC.begin();
        turnPeripheralsOn();
    #endif
}

void loop(){
    if(shouldSleep){
        // Turn off the built-in LED before going to sleep
        digitalWrite(LED_BUILTIN, HIGH);

        #ifdef TURN_PERIPHERALS_OFF
            turnPeripheralsOff();
        #endif
        lowPower.deepSleep(); 
        shouldSleep = false;
    }

    // Blink the built-in LED every 500ms when the device is not sleeping    
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
}




