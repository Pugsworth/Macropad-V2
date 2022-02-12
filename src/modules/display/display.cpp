#include "display.hpp"

#include <SPI.h>
#include <Wire.h>
// #include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);

namespace Display
{
    bool hasUpdate = false;

    void Init()
    {
        if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
        {
            Serial.println("Could not initialize display!!!");
            pinMode(TXLED1, OUTPUT);
            digitalWrite(TXLED1, HIGH);
            for(;;);
        }
        display.display();
        // delay(2000);
        display.clearDisplay();

        display.dim(true);

        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(12, 12);
        display.println("Bonjour!");
        display.display();
    }

    void Update()
    {
    }


    void DrawLayer(const int layer)
    {
        display.clearDisplay();
        display.setCursor(12, 12);
        display.print("Layer: ");
        display.println(layer);
        display.display();
    }
}