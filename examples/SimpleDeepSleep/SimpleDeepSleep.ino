#include "LowPower.h"

#define SLEEP_PIN 0
#define WAKE_PIN A0

LowPower lowPower = LowPower();


void goToSleep(){
    lowPower.sleep(); // lowPower.deepSleep();
}

void setup(){
    attachInterrupt(digitalPinToInterrupt(SLEEP_PIN), goToSleep, RISING);
    lowPower.setWakeupPin(WAKE_PIN);
}

void loop(){
    // do something
}