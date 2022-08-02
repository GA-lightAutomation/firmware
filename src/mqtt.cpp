#include <Arduino.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

const char* mqtt_server = "192.168.100.104";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


void mqttSetup(){
    client.setServer(mqtt_server, 1883);
}

bool runMQTT(String command,String payload){
    /**
     * @brief 
     * Command paramete containers topic to use
     * Payload parameter ... self explanatory
     */
}