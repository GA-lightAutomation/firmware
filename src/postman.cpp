#include <Arduino.h>
#include <string>

#include "globalV.h"
#include "http.h"
#include "mqtt.h"
#include "tcp.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

/* 
Gearbox Members
Members@Gearbox
Happy_home
JMongare@123
FABIAN
Gisore@123
Thee one
kenyans254
*/

const char* ssid     = "Gearbox Members";
const char* password = "Members@Gearbox";
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

void sendData(String command,String payload){
  if(runHTTPclient(command,payload))
    Serial.println("Sent Using HTTP");
  else{
    Serial.println("Falling back to MQTT");
    if(runMQTT(command,payload))
      Serial.println("Sent Using MQTT");
    else
      Serial.println("ERROR, Data not sent");
  }
}