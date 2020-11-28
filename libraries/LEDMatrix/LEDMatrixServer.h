#ifndef LEDMATRIXSERVER_H
#define LEDMATRIXSERVER_H


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define DEFAULT_IP IPAddress(192, 168, 1, 137)

AsyncWebServer server(80);

void onNotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setupWiFi(const char* ssid="LED-Matrix", const char* password="password", bool APMode=false, IPAddress ip = DEFAULT_IP){
  if (APMode){
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00
    WiFi.softAP("Matrix", "password");
  } else{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("WiFi Failed!");
      return;
    } else {
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
    }
  }

  server.onNotFound(onNotFound);
}


#endif