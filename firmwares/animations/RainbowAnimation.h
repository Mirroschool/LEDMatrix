#ifndef RAINBOW_ANIMATION_H
#define RAINBOW_ANIMATION_H

#include <FastLED.h>
#include <LEDMatrix.h>

namespace RainbowAnimation{
  void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
  {
    byte lineStartHue = startHue8;
    for( byte y = 0; y < MATRIX_HEIGHT; y++) {
      lineStartHue += yHueDelta8;
      byte pixelHue = lineStartHue;      
      for( byte x = 0; x < MATRIX_WIDTH; x++) {
        pixelHue += xHueDelta8;
        leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
      }
    }
  }

  unsigned long t = 0; /* keep time */
  void render(uint16_t fps)
  {
    if ( t > millis() ) return;
    t = millis() + (1000 / fps);
      uint32_t ms = millis();
      int32_t yHueDelta32 = ((int32_t)cos16( ms * (27/1) ) * (350 / MATRIX_WIDTH));
      int32_t xHueDelta32 = ((int32_t)cos16( ms * (39/1) ) * (310 / MATRIX_HEIGHT));
      DrawOneFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
      if( ms < 5000 ) {
        FastLED.setBrightness( scale8( BRIGHTNESS, (ms * 256) / 5000));
      } else {
        FastLED.setBrightness(BRIGHTNESS);
      }
      FastLED.show();
  }
}

#endif