#include <Arduino.h>
#include <string>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#include "globalV.h"
#include "mqtt.h"
#include "http.h"

//TODO: Handle when mqtt client on server is not running

/*
Gearbox Members
Members@Gearbox
Happy_home
JMongare@123
FABIAN
Gisore@123
Thee one
kenyans254
Macsc
micron@2022
*/

const char* ssid     = "Macsc";
const char* password = "micron@2022";
//AP credentials
const char* mySSID = "ESP32-AP";
const char* myPassword = "esp32wifi";

bool APflag = false;
void APsetup(){
  WiFi.softAP(mySSID, myPassword);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  APflag = true;
}

void wifiSetup(){
    //start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    int countdown = 10; //countdown to switch to AP

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
        if (countdown==0){ //switch to AP
          APsetup();
          break;
        }
        countdown-=1;
    }

    if (!APflag){
      Serial.println("");
      Serial.println("WiFi connected.");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }


}

void postmanSetup(){
  wifiSetup();
  setupHTTP();
  setupMQTT();
}

void runPostMan(){
  runHTTPserver();
}


void sendData(String command,String payload){
  if(runHTTPclient(command,payload))
    Serial.println("Sent Using HTTP");
  else{
    Serial.println("Falling back to MQTT");
    if(runMQTT(command,payload))
      Serial.println("Sent Using MQTT");
    else
      Serial.println("ERROR, Data("+payload+") not sent");
  }
}