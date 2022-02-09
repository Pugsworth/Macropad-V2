#ifndef __LED_H__
#define __LED_H__

#include <FastLED.h>

namespace LedModule
{
    static const uint8_t LED_COUNT = 13; // ehhhhh TODO: Magic number!
    static const uint8_t PIN_LED_DATA = A2;

    // Instead of keeping various state data in the global namespace, we will use a
    //   data struct to initialize once and pass around in the classic C style!
    struct FastLEDSetupData_t
    {
        uint8_t DataPin;
        uint8_t LedCount;
        CRGB *Leds;
        // FastLEDSetupData_t(uint8_t pin, uint8_t ledcount, CRGB leds[]) : DataPin(pin), LedCount(ledcount), Leds(leds) {}
    };

    struct FastLEDSetupData_t Init();

    void setLedColor(CRGB color);
    void turnOffLeds();
    void fadeall(CRGB leds[], uint8_t count);
    void setall(CRGB leds[], uint8_t count, uint8_t r, uint8_t g, uint8_t b);
}

#endif // __LED_H__