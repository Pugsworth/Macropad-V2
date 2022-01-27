#include <FastLED.h>

int hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
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