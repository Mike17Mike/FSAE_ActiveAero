#include <mbed.h>
#include <neopixel.h>

#define NUM_LEDs        15
#define COLOR_RED       255, 0, 0

//Can use PB_0 for led
DigitalOut led(PB_0);
DigitalOut pixelPin(PB_1);
NeoPixel strip(PB_1, 15);

CAN can1(PA_11, PA_12);

int main() {

    // Set up CAN for no reason
    can1.frequency(250000);

    // Wait 20ms for power to settle before firing up LEDs
    wait_us(20 * 1000);

    // Turn on the dinky LED
    led.write(1);

    while(1) {
        // Turn on the LEDs at full red
        // do this in a loop in case of shenanigans on the dumb WS2812s
        for (int i = 0; i < NUM_LEDs; i++) {
            strip.setPixelColor(i, COLOR_RED);
        }
        strip.show();

        // idk
        wait_us(20 * 1000);
    }
}
