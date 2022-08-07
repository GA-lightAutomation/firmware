#include <Arduino.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "globalV.h"

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
bool receiveFlag = false; //no message received

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    message+=(char)payload[i];
  }
  Serial.println(message);

  receiveFlag=true;
}

void setupMQTT() {
  client.setServer(server_addr, 1883);
  client.setCallback(callback);
}

bool runMQTT(String topic,String message){
  //Attempt connection
  String clientId = "ESP32";
  if (client.connect(clientId.c_str())) {
    if(client.subscribe("ACK")){
      Serial.println("connected");
      client.publish(topic.c_str(), message.c_str()); //hint returns bool
      while(!receiveFlag) client.loop();
      receiveFlag = false;
    }else
      Serial.println("Could not subscribe");
  }else{
    int count = 0;
    while (!client.connected() && count!=3) {
      count++;
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      // Attempt to connect
      if (client.connect(clientId.c_str())) {
        Serial.println("connected");
        // Once connected, publish an announcement...
        client.publish(topic.c_str(), message.c_str());
        // ... and resubscribe
        client.subscribe("inTopic");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 3 seconds");
        // Wait 5 seconds before retrying
        delay(3000);
      }
    }
    return false;
  }

  return true;
}