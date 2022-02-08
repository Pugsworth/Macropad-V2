#include "led.hpp"

namespace LedModule
{
    CRGB leds[LED_COUNT];

    FastLEDSetupData_t fastLEDData = {
        /*.DataPin*/    PIN_LED_DATA,
        /*.LedCount*/   LED_COUNT,
        /*.Leds*/       &leds
    };

    struct FastLEDSetupData_t Init()
    {
        FastLED.addLeds<SK6812, PIN_LED_DATA, GRB>(leds, LED_COUNT);
        FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
        FastLED.setBrightness(200);
        FastLED.clear();

        return fastLEDData;
    }

    void changeGlobalLEDColor(uint16_t value)
    {
        uint8_t hue = map(value, 0, 1024, 0, 256) - 1;

        Serial.print("Changing hue to: ");
        Serial.println(hue);

        CRGB color;
        color.setHSV(hue, 255, 255);

        setall(leds, LED_COUNT, color.r, color.g, color.b);
    }

    void updateLEDs()
    {
        // static int i = 0;
        // static int direction = 1;
        // static int hue = 0;

        // hue %= 256;
        // i %= (int)LED_COUNT;

        // Serial.println(i);

        // leds[i] = CHSV(hue++, 255, 255);
        // FastLED.show();
        // fadeall();

        // i += direction;

        // if (i == LED_COUNT && direction == 1) {
        //   direction = -1;
        // } else if (i == 0 && direction == -1) {
        //   direction = 1;
        // }

        // FastLED.delay(10);

        static int hue = 0;
        static int i = 0;
        static unsigned long nextHueChange = millis() + 30;
        static unsigned long nextIncrement = millis() + 10;
        static unsigned long nextFade = millis() + 3;

        if (millis() > nextHueChange)
        {
            hue = (hue + 1) % 256;
            nextHueChange = millis() + 10;
        }

        if (millis() > nextIncrement)
        {
            i = (i + 1) % LED_COUNT;
            nextIncrement = millis() + 100;

            leds[i] = CHSV(hue, 255, 255);
        }

        if (millis() > nextFade)
        {
            fadeall(leds, LED_COUNT);
            nextFade = millis() + 10;
        }
    }

    void fadeall(CRGB leds[], uint8_t count)
    {
        for (uint8_t i = 0; i < count; i++)
        {
            leds[i].nscale8(250);
        }
    }

    void setall(CRGB leds[], uint8_t count, uint8_t r, uint8_t g, uint8_t b)
    {
        for (uint8_t i = 0; i < count; i++)
        {
            leds[i].setRGB(r, g, b);
        }

        FastLED.show();
    }
}