/**
 * MatrixFireFast - A fire simulation for NeoPixel (and other?) matrix
 * displays on Arduino (or ESP8266) using FastLED.
 *
 * Author: Patrick Rigney (https://www.toggledbits.com/)
 * Copyright 2020 Patrick H. Rigney, All Rights Reserved.
 *
 * Github: https://github.com/toggledbits/MatrixFireFast
 * License information can be found at the above Github link.
 *
 * Please donate in support of my projects: https://www.toggledbits.com/donate
 *
 * For configuration information and processor selection, please see
 * the README file at the above Github link.
 */

#ifndef FIRE_ANIMATION_H
#define FIRE_ANIMATION_H

#include <FastLED.h>
#include <LEDMatrix.h>
namespace FireAnimation{
  /* Display size; can be smaller than matrix size, and if so, you can move the origin.
   * This allows you to have a small fire display on a large matrix sharing the display
   * with other stuff. See README at Github. */
  const uint16_t rows = MATRIX_HEIGHT;
  const uint16_t cols = MATRIX_WIDTH;
  const uint16_t xorg = 0;
  const uint16_t yorg = 0;

  /* Flare constants */
  const uint8_t flarerows = 2;    /* number of rows (from bottom) allowed to flare */
  const uint8_t maxflare = 8;     /* max number of simultaneous flares */
  const uint8_t flarechance = 50; /* chance (%) of a new flare (if there's room) */
  const uint8_t flaredecay = 14;  /* decay rate of flare radiation; 14 is good */

  /* This is the map of colors from coolest (black) to hottest. Want blue flames? Go for it! */
  const uint32_t colors[] = {
    0x000000,
    0x100000,
    0x300000,
    0x600000,
    0x800000,
    0xA00000,
    0xC02000,
    0xC04000,
    0xC06000,
    0xC08000,
    0x807080
  };
  const uint8_t NCOLORS = (sizeof(colors)/sizeof(colors[0]));

  uint8_t pix[rows][cols];
  uint8_t nflare = 0;
  uint32_t flare[maxflare];


  uint32_t isqrt(uint32_t n) {
    if ( n < 2 ) return n;
    uint32_t smallCandidate = isqrt(n >> 2) << 1;
    uint32_t largeCandidate = smallCandidate + 1;
    return (largeCandidate*largeCandidate > n) ? smallCandidate : largeCandidate;
  }

  // Set pixels to intensity around flare
  void glow( int x, int y, int z ) {
    int b = z * 10 / flaredecay + 1;
    for ( int i=(y-b); i<(y+b); ++i ) {
      for ( int j=(x-b); j<(x+b); ++j ) {
        if ( i >=0 && j >= 0 && i < rows && j < cols ) {
          int d = ( flaredecay * isqrt((x-j)*(x-j) + (y-i)*(y-i)) + 5 ) / 10;
          uint8_t n = 0;
          if ( z > d ) n = z - d;
          if ( n > pix[i][j] ) { // can only get brighter
            pix[i][j] = n;
          }
        }
      }
    }
  }

  void newflare() {
    if ( nflare < maxflare && random(1,101) <= flarechance ) {
      int x = random(0, cols);
      int y = random(0, flarerows);
      int z = NCOLORS - 1;
      flare[nflare++] = (z<<16) | (y<<8) | (x&0xff);
      glow( x, y, z );
    }
  }

  /** make_fire() animates the fire display. It should be called from the
   *  loop periodically (at least as often as is required to maintain the
   *  configured refresh rate). Better to call it too often than not enough.
   *  It will not refresh faster than the configured rate. But if you don't
   *  call it frequently enough, the refresh rate may be lower than
   *  configured.
   */
  unsigned long t = 0; /* keep time */
  void make_fire(uint16_t fps) {
    uint16_t i, j;
    if ( t > millis() ) return;
    t = millis() + (1000 / fps);

    // First, move all existing heat points up the display and fade
    for ( i=rows-1; i>0; --i ) {
      for ( j=0; j<cols; ++j ) {
        uint8_t n = 0;
        if ( pix[i-1][j] > 0 )
          n = pix[i-1][j] - 1;
        pix[i][j] = n;
      }
    }

    // Heat the bottom row
    for ( j=0; j<cols; ++j ) {
      i = pix[0][j];
      if ( i > 0 ) {
        pix[0][j] = random(NCOLORS-6, NCOLORS-2);
      }
    }

    // flare
    for ( i=0; i<nflare; ++i ) {
      int x = flare[i] & 0xff;
      int y = (flare[i] >> 8) & 0xff;
      int z = (flare[i] >> 16) & 0xff;
      glow( x, y, z );
      if ( z > 1 ) {
        flare[i] = (flare[i] & 0xffff) | ((z-1)<<16);
      } else {
        // This flare is out
        for ( int j=i+1; j<nflare; ++j ) {
          flare[j-1] = flare[j];
        }
        --nflare;
      }
    }
    newflare();

    // Set and draw
    for ( i=0; i<rows; ++i ) {
      for ( j=0; j<cols; ++j ) {
        leds[XY(j, i, true)] = colors[pix[i][j]];
      }
    }
    FastLED.show();
  }
}
#endif