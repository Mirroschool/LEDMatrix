#include <LEDMatrixServer.h>

#include "FireAnimation.h"
#include "PacificAnimation.h"
#include "FadeAnimation.h"
#include "RainbowAnimation.h"
#include "CylonAnimation.h"

void onSettings(AsyncWebServerRequest *request) {
  char responseBuf[100];
  sprintf(
    responseBuf,
    "{\"modes\":[\"animations\"],\"width\":\"%d\",\"height\":\"%d\"}", // format
    MATRIX_WIDTH,
    MATRIX_HEIGHT
  );
  request->send(200, "application/json", responseBuf);
}

uint16_t currentFPS = 15;
uint8_t currentAnimationId = 1;
uint8_t currentBrightness = 64;

void setup() {
    Serial.begin(115200);
    setupFastLED();
    setupWiFi("OpenWRT", "22446688");

    server.on("/settings/", onSettings);

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/animations/", HTTP_POST, [](AsyncWebServerRequest *request){
        String responseMessage, rawAnimationId;
        int responseCode;

        if (request->hasParam("animation_id", true)) {
            rawAnimationId = request->getParam("animation_id", true)->value();
            currentAnimationId = rawAnimationId.toInt();
            Serial.println(rawAnimationId);
            
            responseMessage = "ok";
            responseCode = 200;
        } else {
            responseMessage = "Malformed request";
            responseCode = 404;
        }

        request->send(responseCode, "text/plain", responseMessage);
    });

    server.on("/fps/", HTTP_POST, [](AsyncWebServerRequest *request){
        String responseMessage, rawFps;
        int responseCode;

        if (request->hasParam("fps", true)) {
            rawFps = request->getParam("fps", true)->value();
            currentFPS = rawFps.toInt();
            Serial.println(currentFPS);
            
            responseMessage = "ok";
            responseCode = 200;
        } else {
            responseMessage = "Malformed request";
            responseCode = 404;
        }

        request->send(responseCode, "text/plain", responseMessage);
    });

    server.on("/brightness/", HTTP_POST, [](AsyncWebServerRequest *request){
        String responseMessage, rawBrightness;
        int responseCode;

        if (request->hasParam("brightness", true)) {
            rawBrightness = request->getParam("brightness", true)->value();
            currentBrightness = rawBrightness.toInt();
            FastLED.setBrightness(currentBrightness);
            Serial.println(currentBrightness);
            
            responseMessage = "ok";
            responseCode = 200;
        } else {
            responseMessage = "Malformed request";
            responseCode = 404;
        }

        request->send(responseCode, "text/plain", responseMessage);
    });

    server.begin();
}

void loop() {
  switch(currentAnimationId){
    case 1:
      FireAnimation::make_fire(currentFPS);
      break;
    case 2:
      PacificAnimation::render(currentFPS);
      break;
    case 3:
      FadeAnimation::render(currentFPS);
      break;
    case 4:
      RainbowAnimation::render(currentFPS);
      break;
    case 5:
      CylonAnimation::render(currentFPS);
      break;
    default:
      FastLED.clear();
      FastLED.show();
      delay(100);
      break;
  }
}
