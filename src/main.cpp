#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "globalV.h"
#include "rfid.h"
#include "strokes.h"
#include "postman.h"

void blinkTest(){
  digitalWrite(blinkPin,HIGH);
  delay(1000);
  digitalWrite(blinkPin,LOW);
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  pinMode(blinkPin,OUTPUT);
  postmanSetup();
}

void loop() {
  //blinkTest();
  runPostMan();
  scanCard();
  keyScanner();
  serialScanner();
}