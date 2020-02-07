#ifndef NEOPIXEL_H
#define NEOPIXEL_H
#include "mbed.h"

union Pixel {
  struct __attribute__((packed)) {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a; // unused
  };
  uint32_t hex;
};

class NeoPixel : DigitalOut {
private:
  void byte(uint32_t b);
  int _npixels;
  Pixel *_pixels;

public:
  NeoPixel(PinName, int);
  void show(void);
  void setColor(int, uint32_t);
  void clear(void);
  void setPixelColor(int pixel, uint8_t red, uint8_t green, uint8_t blue);
};

#endif /* NEOPIXEL_H */