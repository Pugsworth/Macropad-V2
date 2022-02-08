#include "firmware.hpp"
#include <Arduino.h>
#include "modules/led/led.hpp"
#include "modules/mykeypad/mykeypad.hpp"



void Firmware::InitSerial()
{
    Serial.begin(9600);
    Serial.println("Bonjour!");
}


////
// Firmware functions
////

void Firmware::Init()
{
    InitSerial();
    // MyKeypad::InitKeyboard();

    // struct LedModule::FastLEDSetupData_t fastLEDData;
    // fastLEDData = LedModule::Init();
}

void Firmware::Update(unsigned long time)
{
    // readEncoder()
    // update global time
    // btnMode.update(g_time)
    // key_matrix.keyKeys()

    // MyKeypad::Update(time);
}
