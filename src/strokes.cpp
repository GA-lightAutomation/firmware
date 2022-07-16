#include <Arduino.h>
#include "postman.h"
#include "globalV.h"
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {R1, R2, R3, R4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {C1, C2, C3,C4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String PIN;

void keyScanner(){
  char key = keypad.getKey();
  if (key){
    PIN+=key;
  }
  if(PIN.length()==4){
    Serial.print("You enterd: ");
    Serial.println(PIN);
    runClient("keypad",PIN);
    PIN="";
  }
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