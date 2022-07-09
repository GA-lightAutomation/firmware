#include <Arduino.h>
#include "globalV.h"

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
*/

const char* ssid     = "Gearbox Members";
const char* password = "Members@Gearbox";
//AP credentials
const char* mySSID = "ESP32-AP";
const char* myPassword = "esp32wifi";

String headT;
String scriptT;
WiFiServer server(80);

bool APflag = false;
void APsetup(){
  WiFi.softAP(mySSID, myPassword);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  APflag = true;
}

void serverSetup(){
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
    
    
    server.begin();

    //Prepare HTML content
    
    String stylePath = "http://127.0.0.1:5500/complex/style.css";
    stylePath = "https://raw.githubusercontent.com/GA-lightAutomation/interface/model-1/complex/style.css"; // uncomment to use hosted file
    String htmlPath = "http://127.0.0.1:5500/complex/index.html";
    htmlPath = "https://raw.githubusercontent.com/GA-lightAutomation/interface/model-1/complex/index.html"; // uncomment to use hosted file
    
    //head tag
    headT = "<head><link rel=\"stylesheet\" href=\""+stylePath+"\"><title>EazyPizzy</title></head>";
    //script tag
    scriptT = "<script >let postman = new XMLHttpRequest();postman.open(\"GET\", \""+htmlPath+"\", false);postman.send();document.body.innerHTML = postman.responseText;</script></html>";

}

void runServer(){
    WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 400 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<!DOCTYPE html><html lang=\"en\">");
            client.print(headT);
            client.print("");
            client.print("<body><h2>No script</h2>Click <a href=\"/H\">here</a> to turn the LED on pin 2 on.<br> Click <a href=\"/L\">here</a> to turn the LED on pin 2 off.<br></body>");
            client.print(scriptT);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(blinkLED, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(blinkLED, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}