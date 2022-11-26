// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

// Replace with your network credentials
const char* ssid = "NodeMCU";
const char* password = "00000000";

// Set LED PIN
const int ledPin = D1;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    digitalRead(ledPin) ?  ledState = "ON" : ledState = "OFF";
    Serial.print(ledState);
    return ledState;
  }
}
 
void setup(){
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // initializing SPIFFS 
  SPIFFS.begin() ? Serial.println("mounting SPIFFS.....") : Serial.println("Error while mounting SPIFFS");

  // wifi softAp 
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("Access Point IP:");
  Serial.println(myIP);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  
  // Route to load png file
  server.on("/onn.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/onn.png", "text/css");
  });

 // Route to load png file
  server.on("/off.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/off.png", "text/css");
  });
  
  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
