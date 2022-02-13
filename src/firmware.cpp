#include <Arduino.h>
#include "firmware.hpp"
#include "modules/led/led.hpp"
#include "modules/mykeypad/mykeypad.hpp"
#include "modules/display/display.hpp"


void Firmware::InitSerial()
{
    Serial.begin(9600);
    delay(500);
    Serial.println("Bonjour!");
    delay(500);
}





////
// Firmware functions
////

void Firmware::Init()
{
    InitSerial();

    int seed = analogRead(0);
    Serial.print("Initializing random with seed: ");
    Serial.println(seed);
    srand(seed);

    Display::Init();

    MyKeypad::InitKeyboard();
    MyKeypad::layerChange += [](uint8_t layer){
        Display::DrawLayer(layer);
    };

    // struct LedModule::FastLEDSetupData_t fastLEDData;
    // fastLEDData = LedModule::Init();
}

void Firmware::Update(unsigned long time)
{
    MyKeypad::Update(time);
    Display::Update(time);
}
