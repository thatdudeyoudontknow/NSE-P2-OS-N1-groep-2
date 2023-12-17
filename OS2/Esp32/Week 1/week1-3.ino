#include <WiFi.h>
#include "secret.h"
#include "time.h"
// data below is set as a define in the file secret.h
//const char* ssid = "HANZE-ZP11";
//const char* password = "dby555YcV2";
void WiFi_connect(){
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  // Print timestamp
  Serial.print(millis());
  Serial.print(": ");

  // Print "Connected" message on a new line
  Serial.println("Connected");
  }
  Serial.print("Connected to WiFi: ");
  Serial.println(WiFi.SSID());
  }
void setup(){
  Serial.begin(115200);
  WiFi_connect();
  }
void loop(){
  

}