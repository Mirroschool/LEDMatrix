#ifndef CYLON_ANIMATION_H
#define CYLON_ANIMATION_H

#include <FastLED.h>
#include <LEDMatrix.h>

namespace CylonAnimation{
  CRGBPalette16 currentPalette;
  CRGBPalette16 targetPalette;
  TBlendType    currentBlending = LINEARBLEND;                                // NOBLEND or LINEARBLEND


  // Define variables used by the sequences.
  uint8_t thisbeat =  23;                                       // Beats per minute for first part of dot.
  uint8_t thatbeat =  28;                                       // Combined the above with this one.
  uint8_t thisfade =   32;                                       // How quickly does it fade? Lower = slower fade rate.
  uint8_t  thissat = 255;                                       // The saturation, where 255 = brilliant colours.
  uint8_t  thisbri = 255;                                       // Brightness of a sequence.
  int        myhue =   50;

  // Updated sinelon (no visual gaps)
void sinelon()
{
  // a colored dot sweeping 
  // back and forth, with 
  // fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(5,0,NUM_LEDS);
  static int prevpos = 0;
  if( pos < prevpos ) { 
    fill_solid( leds+pos, (prevpos-pos)+1, CHSV(myhue,220,255));
  } else { 
    fill_solid( leds+prevpos, (pos-prevpos)+1, CHSV( myhue,220,255));
  }
  prevpos = pos;
}

  unsigned long t = 0; /* keep time */
  void render(uint16_t fps)
  {
    if ( t > millis() ) return;
    t = millis() + (1000 / fps);

    uint8_t maxChanges = 12; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  
                                      // Change the target palette to a random one every 5 seconds.
    static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  

    sinelon();                                                // Call our sequence.


  FastLED.show();

  }

}

#endif
