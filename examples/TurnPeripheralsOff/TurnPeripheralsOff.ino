#include "Arduino_LowPowerPortentaC33.h"
#include "Arduino_PMIC.h"
#include "Wire.h"

#define SLEEP_PIN 0
#define WAKE_PIN A3

LowPower lowPower;


void goToSleep(){
    PMIC.getControl() -> turnLDO1Off(Ldo1Mode::Normal); // LDO1 is used for AVCC (Analog Power)
    PMIC.getControl() -> turnLDO2Off(Ldo2Mode::Normal); // LDO2 is used for the analog voltage reference
    PMIC.getControl() -> turnLDO3Off(Ldo3Mode::Normal); // LDO3 is a 1.2V regulator used by the Ethernet chip
    PMIC.getControl() -> turnSw1Off(Sw1Mode::Normal);   // SW1 is a 3.3V regulator used by RGB Led, WIFI Chip, Secure element
    PMIC.getControl() -> turnSw2Off(Sw2Mode::Normal);   // SW2 is the power regulator used for powering external circuits (Labeled 3V3 on the board)
    
    lowPower.deepSleep(); 
}

void setup(){
    lowPower = LowPower();
    attachInterrupt(digitalPinToInterrupt(SLEEP_PIN), goToSleep, RISING);
    lowPower.enableWakeupFromPin(WAKE_PIN, RISING);
    pinMode(LED_BUILTIN, OUTPUT);

    PMIC.begin();
    PMIC.getControl() -> turnLDO1On(Ldo1Mode::Normal); // LDO1 is used for AVCC (Analog Power)
    PMIC.getControl() -> turnLDO2On(Ldo2Mode::Normal); // LDO2 is used for the analog voltage reference
    PMIC.getControl() -> turnLDO3On(Ldo3Mode::Normal); // LDO3 is a 1.2V regulator used by the Ethernet chip
    PMIC.getControl() -> turnSw1On(Sw1Mode::Normal);   // SW1 is a 3.3V regulator used by RGB Led, WIFI Chip, Secure element
    PMIC.getControl() -> turnSw2On(Sw2Mode::Normal);   // SW2 is the power regulator used for powering external circuits (Labeled 3V3 on the board)
}

void loop(){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}




