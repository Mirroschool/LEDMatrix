#include <LEDMatrixServer.h>
#include <LEDMatrix.h>
#include <FastLED.h>

#include "pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "pixel.pb.h"

AsyncWebSocket canvasWS("/canvas/ws");


void onEvent(AsyncWebSocket       *server,
             AsyncWebSocketClient *client,
             AwsEventType          type,
             void                 *arg,
             uint8_t              *data,
             size_t                len) {

  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:{
      AwsFrameInfo * frameInfo = (AwsFrameInfo*)arg;
      if(frameInfo->final && frameInfo->index == 0 && frameInfo->len == len){
        PixelMessage message = PixelMessage_init_zero;
        pb_istream_t inputStream = pb_istream_from_buffer(data, len);
        bool status = pb_decode(&inputStream, PixelMessage_fields, &message);
  
        if (!status)
        {
          Serial.printf("Decoding failed: %s\n", PB_GET_ERROR(&inputStream));
          return;
        }
        
        leds[XY(message.x, message.y)] = CRGB(message.red, message.green, message.blue);
        FastLED.show();
      }
      break;
    }

    case WS_EVT_PONG:
      break;
    case WS_EVT_ERROR:
      break;
    default:
      break;
  }
}

void onSettings(AsyncWebServerRequest *request) {
  Serial.println("Settings requested");
  char responseBuf[100];
  sprintf(
    responseBuf,
    "{\"modes\":[\"canvas\"],\"width\":\"%d\",\"height\":\"%d\"}", // format
    MATRIX_WIDTH,
    MATRIX_HEIGHT
  );
  request->send(200, "application/json", responseBuf);
}

void setup(){
  Serial.begin(115200);
  randomSeed(analogRead(A0));
  setupFastLED();
  setupWiFi("OpenWRT", "22446688");
  
  canvasWS.onEvent(onEvent);
  server.addHandler(&canvasWS);
  server.on("/settings/", onSettings);
  server.begin();
}

void loop(){
  canvasWS.cleanupClients();
}
