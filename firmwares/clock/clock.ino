#include <LEDMatrix.h>
#include <FastLED.h>
#include <TimeLib.h>
#include <LEDMatrixServer.h>
#include "font.h"

#include "pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "clock.pb.h"

class Activity {
 public:
  CRGB m_backgroundColor = CRGB::Black;
  CRGB m_textColor = CRGB::White;
  void virtual render();

  void drawDigit(uint8_t digit, uint8_t x, uint8_t y) {
    if ((digit >= 0) && (digit < 10)) {
      uint8_t glyphIndex = getGlyphIndexByCodePoint(48 + digit);
      for (uint8_t i = 0; i < 5; i++) {
        uint8_t glyphLine = pgm_read_byte(&fontGlyphs[glyphIndex][i]);
        drawGlyphLine(glyphLine, x + i, y);
      }
    }
  }

  void drawGlyphLine(uint8_t glyphLine, uint8_t x, uint8_t y) {
    CRGB charPointColor;
    for (uint8_t dy = 0; dy < 8; dy++) {
      charPointColor = bitRead(glyphLine, dy) ? m_textColor : m_backgroundColor;
      leds[XY(x, y + dy, false)] = charPointColor;
    }
  }
};

class CreepingLine : public Activity {
 public:
  CreepingLine(){};
  CreepingLine(uint8_t creepingText[], uint8_t textLength) {
    setText(creepingText, textLength);
  }

  void setFPS(uint8_t fps) { m_fps = fps; }

  void setText(uint8_t creepingText[], uint8_t textLength) {
    m_offset = 0;
    FastLED.clear();
    memset(m_linesBuffer, 0, MATRIX_WIDTH);

    uint16_t linesCount = MATRIX_WIDTH;
    for (uint8_t i = 0; i < textLength; i++) {
      uint8_t codePoint = creepingText[i];
      for (uint8_t j = 0; j < 5; j++) {
        uint8_t glyphLine =
            pgm_read_byte(&fontGlyphs[getGlyphIndexByCodePoint(codePoint)][j]);
        m_linesBuffer[linesCount] = glyphLine;
        linesCount++;
      }
      m_linesBuffer[linesCount] = 0;
      linesCount++;
    }

    memset(&m_linesBuffer[linesCount], 0, MATRIX_WIDTH);
    linesCount += MATRIX_WIDTH;

    m_length = linesCount;
  }

  void render() {
    if (millis() - m_fpsTimer > 1000 / m_fps) {
      m_fpsTimer = millis();
      for (uint8_t i = 0; i < MATRIX_WIDTH; i++) {
        drawGlyphLine(m_linesBuffer[MATRIX_WIDTH + m_offset - i],
                      MATRIX_WIDTH - i - 1, m_offsetY);
      }

      if (++m_offset >= m_length) {
        m_offset = 0;
      }
      FastLED.show();
    }
  }

 private:
  uint8_t
      m_linesBuffer[MATRIX_HEIGHT * MATRIX_WIDTH * 5 + 255 + MATRIX_WIDTH * 2];
  uint8_t m_offsetY = 4;
  uint8_t m_fps = 14;
  uint16_t m_length, m_offset;
  uint32_t m_fpsTimer;
};

class Stopwatch : public Activity {
 public:
  Stopwatch(){};

  void reset() {
    FastLED.clear();
    FastLED.show();
    m_lastSecondTimer = millis();
    m_currentSeconds = 0;
  }

  void render() {
    if (millis() - m_lastSecondTimer > 1000) {
      m_lastSecondTimer = millis();
      if (++m_currentSeconds > 999) {
        m_currentSeconds = 0;
      }

      drawDigit(m_currentSeconds / 100, 0, m_offsetY);
      drawDigit((m_currentSeconds % 100) / 10, 5, m_offsetY);
      drawDigit(m_currentSeconds % 10, 11, m_offsetY);
      FastLED.show();
    }
  }

 private:
  uint8_t m_offsetY = 4;
  uint16_t m_currentSeconds;
  uint32_t m_lastSecondTimer;
};

class Clock : public Activity {
 public:
  Clock(){};

  void setTime(time_t unixtime) {
    FastLED.clear();
    FastLED.show();
    m_time = unixtime;
    m_lastSecondTimer = millis();
  }

  void render() {
    if (millis() - m_lastSecondTimer > 1000) {
      m_lastSecondTimer = millis();
      m_divider = !m_divider;
      m_time++;

      drawDigit(hour(m_time) / 10, 2, 0);
      drawDigit(hour(m_time) % 10, 8, 0);

      drawDigit(minute(m_time) / 10, 2, 9);
      drawDigit(minute(m_time) % 10, 8, 9);

      CRGB dividerColor = m_divider ? m_textColor : m_backgroundColor;

      leds[XY(0, 0)] = dividerColor;
      leds[XY(15, 0)] = dividerColor;
      leds[XY(0, 15)] = dividerColor;
      leds[XY(15, 15)] = dividerColor;
      FastLED.show();
    }
  }

 private:
  uint8_t m_offsetY = 4;
  uint32_t m_lastSecondTimer;
  time_t m_time;
  bool m_divider;
};

Activity* currentActivity;

CreepingLine line;
Stopwatch stopwatch;
Clock clock1;

void onModeChange(AsyncWebServerRequest* request) {
  String responseMessage, rawModeId;
  int responseCode;

  if (request->hasParam("mode_id", true)) {
    rawModeId = request->getParam("mode_id", true)->value();
    Serial.println("RAAAAAAW");
    Serial.println(rawModeId);
    uint8_t modeId = rawModeId.toInt();
    Serial.println(modeId);
    switch (modeId) {
      case 1:
        Serial.println(request->params());
        if (request->hasParam("text", true)) {
          String text = request->getParam("text", true)->value();
          Serial.println(text);

          uint8_t cp1251Buf[255], bufId, currentByte;

          for (int i = 0; i < text.length(); i++) {
            currentByte = text[i];
            Serial.println(currentByte);

            line.setText((uint8_t*)&text[0], text.length());
            currentActivity = &line;

            responseMessage = "ok";
            responseCode = 200;
          }
          else {
            responseMessage = "Bad text";
            responseCode = 404;
          }

          break;

          case 2:
            currentActivity = &stopwatch;
            stopwatch.reset();

            responseMessage = "ok";
            responseCode = 200;
            break;

          case 3:
            if (request->hasParam("time", true)) {
              uint32_t timestamp =
                  request->getParam("time", true)->value().toInt();
              clock1.setTime(timestamp);
              currentActivity = &clock1;

              responseMessage = "ok";
              responseCode = 200;
            } else {
              responseMessage = "Bad time";
              responseCode = 404;
            }
            break;
          default:
            responseMessage = "Bad mode";
            responseCode = 404;
            break;
        }
    }
    else {
      responseMessage = "Malformed request";
      responseCode = 404;
    }

    request->send(responseCode, "text/plain", responseMessage);
  }

  void onFPSChange(AsyncWebServerRequest * request) {
    String responseMessage, rawFPS;
    int responseCode;

    if (request->hasParam("fps", true)) {
      rawFPS = request->getParam("fps", true)->value();
      uint8_t fps = rawFPS.toInt();
      line.setFPS(fps);
      responseMessage = "ok";
      responseCode = 200;
    } else {
      responseMessage = "Malformed request";
      responseCode = 404;
    }

    request->send(responseCode, "text/plain", responseMessage);
  }

  void onSettings(AsyncWebServerRequest * request) {
    char responseBuf[100];
    sprintf(responseBuf,
            "{\"modes\":[\"clock\", \"stopwatch\", "
            "\"creepingLine\"],\"width\":\"%d\",\"height\":\"%d\"}",  // format
            MATRIX_WIDTH,
            MATRIX_HEIGHT);
    request->send(200, "application/json", responseBuf);
  }

  AsyncWebSocket clockModeWS("/clock/ws");

  void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client,
               AwsEventType type, void* arg, uint8_t* data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(),
                      client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA: {
        AwsFrameInfo* frameInfo = (AwsFrameInfo*)arg;
        if (frameInfo->final && frameInfo->index == 0 &&
            frameInfo->len == len) {
          ClockMessage message = ClockMessage_init_zero;
          pb_istream_t inputStream = pb_istream_from_buffer(data, len);
          bool status = pb_decode(&inputStream, ClockMessage_fields, &message);

          if (!status) {
            Serial.printf("Decoding failed: %s\n", PB_GET_ERROR(&inputStream));
            return;
          }

          case 1:
            line.setText(message.text, message.text.length);
            currentActivity = &line;
            break;

          case 2:
            currentActivity = &stopwatch;
            stopwatch.reset();

            responseMessage = "ok";
            responseCode = 200;
            break;

          case 3:
            if (request->hasParam("time", true)) {
              uint32_t timestamp =
                  request->getParam("time", true)->value().toInt();
              clock1.setTime(timestamp);
              currentActivity = &clock1;

              responseMessage = "ok";
              responseCode = 200;
            } else {
              responseMessage = "Bad time";
              responseCode = 404;
            }
            break;
          default:
            responseMessage = "Bad mode";
            responseCode = 404;
            break;
        }
      } break;
    }

    case WS_EVT_PONG:
      break;
    case WS_EVT_ERROR:
      break;
    default:
      break;
  }

  void setup() {
    Serial.begin(115200);
    setupFastLED();
    setupWiFi("OpenWRT", "22446688");

    server.on("/settings/", onSettings);
    server.on("/fps/", HTTP_POST, onFPSChange);
    server.on("/modes/", HTTP_POST, onModeChange);
    server.begin();

    currentActivity = &stopwatch;
  }

  void loop() {
    clockModeWS.cleanupClients();
    currentActivity->render();
  }
