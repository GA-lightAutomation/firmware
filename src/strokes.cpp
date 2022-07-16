#include <Arduino.h>
#include "postman.h"
#include "Adafruit_Keypad.h"


#define KEYPAD_PID3844

#define R1    12
#define R2    11
#define R3    10
#define R4    9
#define C1    8
#define C2    7
#define C3    6
#define C4    5
// leave this import after the above configuration
#include "keypad_config.h"

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String PIN;

void keysScanner(){
  // put your main code here, to run repeatedly:
  customKeypad.tick();

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    Serial.print((char)e.bit.KEY);
    if(e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed");
    else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
  }

  delay(10);
}
void serialScanner(){//get input from serial monitor for testing
  String key;
  while (Serial.available() != 0) {
    Serial.print("You entered: ");
    key = Serial.readString();
    Serial.println(key);
  }
  
  if (key){
    PIN+=key;
  }
  if(PIN.length()==4){
    runClient("keypad",PIN);
    PIN="";
  }
}