#ifndef PLASMA_H
#define PLASMA_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>


/*
Class for the plasma effect
*/

//Sine table to avoid using sin() and improve compute speed
const int8_t sinTable[256] PROGMEM = {
  0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 59, 62, 65, 67, 70, 73, 75, 78, 80, 82, 85, 87, 89, 91, 94, 96, 98, 100, 102, 103, 105, 107, 108, 110, 111, 113, 114, 116, 117, 118, 119, 120, 121, 122, 123, 123, 124, 125, 125, 126, 126, 126, 126, 127, 127, 127, 126, 126, 126, 126, 125, 125, 124, 123, 123, 122, 121, 120, 119, 118, 117, 116, 114, 113, 111, 110, 108, 107, 105, 103, 102, 100, 98, 96, 94, 91, 89, 87, 85, 82, 80, 78, 75, 73, 70, 67, 65, 62, 59, 57, 54, 51, 48, 45, 42, 39, 36, 33, 30, 27, 24, 21, 18, 15, 12, 9, 6, 3, 0, -3, -6, -9, -12, -15, -18, -21, -24, -27, -30, -33, -36, -39, -42, -45, -48, -51, -54, -57, -59, -62, -65, -67, -70, -73, -75, -78, -80, -82, -85, -87, -89, -91, -94, -96, -98, -100, -102, -103, -105, -107, -108, -110, -111, -113, -114, -116, -117, -118, -119, -120, -121, -122, -123, -123, -124, -125, -125, -126, -126, -126, -126, -127, -127, -127, -126, -126, -126, -126, -125, -125, -124, -123, -123, -122, -121, -120, -119, -118, -117, -116, -114, -113, -111, -110, -108, -107, -105, -103, -102, -100, -98, -96, -94, -91, -89, -87, -85, -82, -80, -78, -75, -73, -70, -67, -65, -62, -59, -57, -54, -51, -48, -45, -42, -39, -36, -33, -30, -27, -24, -21, -18, -15, -12, -9, -6, -3
};

enum PlasmaPalette { FIRE,
                     OCEAN,
                     ACID,
                     PSYCHEDELIC,
                     CUSTOM };

class PlasmaEffect {
private:
  Adafruit_ST7735& _tft;
  uint16_t _frame;
  uint8_t _step;
  uint8_t _speed;
  uint8_t _colorIntensity;
  PlasmaPalette _currentPalette;
  int8_t _dir1 = 1, _dir2 = -1, _dir3 = 1; // Direction multipliers

public:
  // Constructor
  PlasmaEffect(Adafruit_ST7735& tft_ref)
    : _tft(tft_ref) {
    _frame = 0;
    _step = 4;  // Block size for performance
    _speed = 4;
    _colorIntensity = 1;
    _currentPalette = FIRE;
  }

  //Change color palette
  void setPalette(PlasmaPalette p) {
    _currentPalette = p;
  }

  //Change the direction of the plasma effect to have different patterns
  void randomizeDirections() {
    _dir1 = (random(0, 2) == 0) ? 1 : -1; // Randomly pick 1 or -1
    _dir2 = (random(0, 2) == 0) ? 1 : -1;
    _dir3 = (random(0, 2) == 0) ? 1 : -1;
  }

  //Change the speed of the plasma effect
  void setSpeed(uint8_t speed) {
    _speed = speed;
    _frame = 0;
  }

  //Change the resolution of the plasma effect
  //A higher number means a more pixelated resolution, yet faster computation
  void setResolution(uint8_t resolution) {
    _step = resolution;
  }

  //Shift the color of the presets
  void setColorIntensity(uint8_t colorIntensity) {
    _colorIntensity = colorIntensity;
  }

  void update() {
    _frame += _speed;

    for (int16_t y = 0; y < 128; y += _step) {
      for (int16_t x = 0; x < 128; x += _step) {

        //Calculate the different sine waves
        int8_t v1 = (int8_t)pgm_read_byte(&sinTable[(x * 2 + _frame*_dir1) & 0xFF]);
        int8_t v2 = (int8_t)pgm_read_byte(&sinTable[(y * 3 - _frame*_dir2) & 0xFF]);
        int8_t v3 = (int8_t)pgm_read_byte(&sinTable[((x + y) + (_frame*_dir3 >> 1)) & 0xFF]);

        //index is like the sine wave, the higher the number is,
        // higher the color is going to be
        uint8_t index = (uint8_t)((v1 + v2 + v3) / 3 + 128);

        uint16_t color = _computeColor(index);

        //Draw a rectangle based on the resolution
        _tft.fillRect(x, y, _step, _step, color);
      }
    }
    // Explicit wrap at 512 to keep Wave 3 perfectly seamless
    if (_frame >= 512) _frame = 0;
  }

private:
  uint16_t _computeColor(uint8_t i) {
    uint8_t r, g, b;
    switch (_currentPalette) { //Switch between the different color palettes.
      case FIRE:
        r = i*_colorIntensity;
        g = i*_colorIntensity >> 2;
        b = 0;
        break;
      case OCEAN:
        r = 0;
        g = i*_colorIntensity >> 1;
        b = i;
        break;
      case ACID:
        r = 200 - i*_colorIntensity;
        g = 254 - i*_colorIntensity;
        b = i*_colorIntensity << 1;  //r = i << 2; g = 255 - i; b = i << 1;
        break;
      case PSYCHEDELIC:
        r = i*_colorIntensity << 3;
        g = i*_colorIntensity << 2;
        b = i*_colorIntensity << 1;
        break;
      default:
        r = g = b = i;
    }
    return _tft.color565(r, g, b);
  }
};

#endif