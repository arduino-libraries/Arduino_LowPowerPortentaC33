#include "Arduino_Portenta_C33_LowPower.h"

#define SLEEP_PIN 0
#define WAKE_PIN A3

LowPower lowPower;


void goToSleep(){
    lowPower.sleep(); // lowPower.deepSleep();
}

void setup(){
    lowPower = LowPower();
    attachInterrupt(digitalPinToInterrupt(SLEEP_PIN), goToSleep, RISING);
    lowPower.enableWakeupFromPin(WAKE_PIN, RISING);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}