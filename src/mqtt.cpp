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
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void reconnect() {
    //loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        //Create a random client ID
        String clientID = "ESP32Client-";
        clientID += String(random(0xffff), HEX);
        //Attempt to connect
        if (client.connect(clientID.c_str())) {
            Serial.println("connected");
            //once connected, publish an announcement...
            client.publish("outTopic", "payload");
            // ... and resubscribe
            client.subscribe("inTopic");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println("try again in 5 seconds");
            //wait 5 seconds before retrying
            delay(5000);
        }
    }

}

void loop () {
    if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "payload #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
