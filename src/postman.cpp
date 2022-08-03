#include <Arduino.h>
#include <string>

#include "globalV.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <PubSubClient.h>

//TODO: Handle broker reset error

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

const char* ssid     = "FABIAN";
const char* password = "Gisore@123";
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

/*
bool runMQTT(String topic,String message)
###################################################################
#                          START MQTT                             #
###################################################################
*/

const char* mqtt_server = "192.168.100.12";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(blinkPin, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(blinkPin, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void setupMQTT() {
  pinMode(blinkPin, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

bool runMQTT(String topic,String message){
  //Attempt connection
  String clientId = "ESP32";
  if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish(topic.c_str(), message.c_str());
      client.subscribe("ACK");
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

  client.loop();

  client.subscribe("ACK");

  return true;
}
/*
###################################################################
#                                   END MQTT                      #
###################################################################
*/


/*
###################################################################
#                                 START HTTP                      #
###################################################################
*/
WiFiServer server(80);
void setupHTTP(){
    server.begin();
}

void runHTTPserver(){
    WiFiClient client = server.available();   // listen for incoming clients

  if(client){
    Serial.println("New Client.");
    while (client.connected()){
      String request;
      if(client.available()){ //receive client request
        request = client.readString();
        Serial.println(request);
      }
      String identifier = "Command: ";
      String command;
      if(request.indexOf(identifier)>0){
        //extract actual command
        int charIndex = request.indexOf(identifier)+identifier.length();
        while(request.charAt(charIndex)!='\r'){
          command+=request.charAt(charIndex);
          charIndex++;
        }
        //send response
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/json");
        client.println();//end of header

        if(command=="Test")
          client.print("{\"Test\":\"Hello server\"}");
        else if(command=="OFF"){
          digitalWrite(blinkPin,HIGH);
          client.print("{\"Response\":\"Light OFF\"}");
        }
        else if(command=="ON"){
          digitalWrite(blinkPin,LOW);
          client.print("{\"Response\":\"Light ON\"}");
        }
        else if(command=="authenticate"){
          Serial.println("DOORS OPEN");
          client.print("{\"Response\":\"Access Granted\"}");
        }
        else if(command=="denied"){
          Serial.println("NO SUCH USER");
          client.print("{\"Response\":\"Access Denied\"}");
        }
        else
          client.print("{\"Error\":\"Command does not exist\"}");

        client.println();//end of response
        break;
      }
      if(request.indexOf(identifier)<0){
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();

        // the content of the HTTP response follows the header:
        client.print("<!DOCTYPE html><html lang=\"en\">");
        client.print("<body><h2>No script</h2>Click <a href=\"/H\">here</a> to turn the LED on pin 2 on.<br> Click <a href=\"/L\">here</a> to turn the LED on pin 2 off.<br></body>");

        // The HTTP response ends with another blank line:
        client.println();
        // break out of the while loop:
        break;
      }
    }

  }
}

bool runHTTPclient(String command,String payload){
  const int    HTTP_PORT   = 5000;
  const String HTTP_METHOD = "GET"; // or "POST"
  const char   HOST_NAME[] = "192.168.0.203"; // hostname of web server:
  const String PATH_NAME   = "";
  String queryString = "/esp?command="+command+"&payload="+payload;

  WiFiClient client;
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to server");
    // send HTTP request header
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header
    return true;
  } else {
    Serial.println("HTTP connection failed");
    return false;
  }
}

/*
###################################################################
#                               END HTTP                          #
###################################################################
*/

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