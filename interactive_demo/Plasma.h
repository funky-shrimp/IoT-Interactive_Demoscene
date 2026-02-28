#ifndef PLASMA_H
#define PLASMA_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

//Sine table to avoid using sin() and improve compute speed
const int8_t sinTable[256] PROGMEM = {
  0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 59, 62, 65, 67, 70, 73, 75, 78, 80, 82, 85, 87, 89, 91, 94, 96, 98, 100, 102, 103, 105, 107, 108, 110, 111, 113, 114, 116, 117, 118, 119, 120, 121, 122, 123, 123, 124, 125, 125, 126, 126, 126, 126, 127, 127, 127, 126, 126, 126, 126, 125, 125, 124, 123, 123, 122, 121, 120, 119, 118, 117, 116, 114, 113, 111, 110, 108, 107, 105, 103, 102, 100, 98, 96, 94, 91, 89, 87, 85, 82, 80, 78, 75, 73, 70, 67, 65, 62, 59, 57, 54, 51, 48, 45, 42, 39, 36, 33, 30, 27, 24, 21, 18, 15, 12, 9, 6, 3, 0, -3, -6, -9, -12, -15, -18, -21, -24, -27, -30, -33, -36, -39, -42, -45, -48, -51, -54, -57, -59, -62, -65, -67, -70, -73, -75, -78, -80, -82, -85, -87, -89, -91, -94, -96, -98, -100, -102, -103, -105, -107, -108, -110, -111, -113, -114, -116, -117, -118, -119, -120, -121, -122, -123, -123, -124, -125, -125, -126, -126, -126, -126, -127, -127, -127, -126, -126, -126, -126, -125, -125, -124, -123, -123, -122, -121, -120, -119, -118, -117, -116, -114, -113, -111, -110, -108, -107, -105, -103, -102, -100, -98, -96, -94, -91, -89, -87, -85, -82, -80, -78, -75, -73, -70, -67, -65, -62, -59, -57, -54, -51, -48, -45, -42, -39, -36, -33, -30, -27, -24, -21, -18, -15, -12, -9, -6, -3
};

enum PlasmaPalette { FIRE, OCEAN, ACID, PSYCHEDELIC };

class PlasmaEffect {
  private:
    Adafruit_ST7735& _tft;
    uint8_t _frame;
    uint8_t _step;
    PlasmaPalette _currentPalette;

  public:
    // Constructor
    PlasmaEffect(Adafruit_ST7735& tft_ref) : _tft(tft_ref) {
      _frame = 0;
      _step = 4; // Block size for performance
      _currentPalette = FIRE;
    }

    void setPalette(PlasmaPalette p) {
      _currentPalette = p;
    }

    void setResolution(uint8_t s) {
      _step = s;
    }

    void update() {
      _frame += 5;
      
      for (int16_t y = 0; y < 128; y += _step) {
        for (int16_t x = 0; x < 128; x += _step) {
          
          int8_t v1 = (int8_t)pgm_read_byte(&sinTable[(x * 2 + _frame) & 0xFF]);
          int8_t v2 = (int8_t)pgm_read_byte(&sinTable[(y * 3 - _frame) & 0xFF]);
          int8_t v3 = (int8_t)pgm_read_byte(&sinTable[((x + y) + (_frame >> 1)) & 0xFF]);
          
          uint8_t index = (uint8_t)((v1 + v2 + v3) / 3 + 128);
          uint16_t color = _computeColor(index);
          
          _tft.fillRect(x, y, _step, _step, color);
        }
      }
    }

  private:
    uint16_t _computeColor(uint8_t i) {
      uint8_t r, g, b;
      switch (_currentPalette) {
        case FIRE:
          r = i; g = i >> 2; b = 0;
          break;
        case OCEAN:
          r = 0; g = i >> 1; b = i;
          break;
        case ACID:
          r = i << 2; g = 255 - i; b = i << 1;
          break;
        case PSYCHEDELIC:
          r = i << 3; g = i << 2; b = i << 1;
          break;
        default:
          r = g = b = i;
      }
      return _tft.color565(r, g, b);
    }
};

#endif