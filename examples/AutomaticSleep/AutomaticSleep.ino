#include "LowPower.h"
LowPower lowPower  = LowPower();

void setup(){
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){

    digitalWrite(LED_BUILTIN, LOW);
    lowPower.delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    lowPower.delay(100);
}
