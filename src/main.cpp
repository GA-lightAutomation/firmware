#include <Arduino.h>
#include "globalV.h"
#include "wifiServer.h"
#include "BTsupport.h"

void setup()
{
    Serial.begin(115200);
    pinMode(blinkLED, OUTPUT);      // set the LED pin mode

    delay(10);
    
    serverSetup();
}

int value = 0;

void loop(){
  runServer();
}