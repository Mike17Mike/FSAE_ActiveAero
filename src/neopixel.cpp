#include "neopixel.h"
#include "mbed.h"

NeoPixel::NeoPixel(PinName pin, int n) : DigitalOut(pin) {
  _pixels = (Pixel *)malloc(n * sizeof(Pixel));
  memset(_pixels, 0, n * sizeof(Pixel));
  _npixels = n;
}

void nop() {
  for (int i = 0; i < 2; i++) {
    __NOP();
  }
}

// The timing should be approximately 800ns/300ns, 300ns/800ns
void NeoPixel::byte(register uint32_t byte) {
  for (int i = 0; i < 8; i++) {
    gpio_write(&gpio, 1);

    // duty cycle determines bit value
    if (byte & 0x80) {
      // one
      for (int j = 0; j < 6; j++)
        nop();

      gpio_write(&gpio, 0);
      for (int j = 0; j < 2; j++)
        nop();

    } else {
      // zero
      for (int j = 0; j < 2; j++)
        nop();

      gpio_write(&gpio, 0);
      for (int j = 0; j < 5; j++)
        nop();
    }

    byte = byte << 1; // shift to next bit
  }
}

void NeoPixel::show(void) {
  // Disable interrupts in the critical section
  __disable_irq();

  for (int i = 0; i < _npixels; i++) {
// Black magic to fix distorted timing
#ifdef __HAL_FLASH_INSTRUCTION_CACHE_DISABLE
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();
#endif

    byte((_pixels + i)->g);
    byte((_pixels + i)->r);
    byte((_pixels + i)->b);

#ifdef __HAL_FLASH_INSTRUCTION_CACHE_ENABLE
    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
#endif
  }

  __enable_irq();
  wait_us(50);
}

void NeoPixel::setColor(int i, uint32_t color) {
  if ((i >= 0) && (i < _npixels))
    (_pixels + i)->hex = color;
}

void NeoPixel::setPixelColor(int pixelNumber, uint8_t r, uint8_t g, uint8_t b) {
  setColor(pixelNumber, uint32_t((r << 16) + (g << 8) + b));
}

void NeoPixel::clear(void) {
  for (int i = 0; i < _npixels; i++)
    (_pixels + i)->hex = 0;
}