#include "firmware.h"



void InitSerial()
{
    Serial.begin(9600);
    Serial.println("Bonjour!");
}


////
// Firmware functions
////

void Firmware::init()
{
    InitSerial();
    InitKeyboard();

    struct FastLEDSetupData_t fastLEDData;
    fastLEDData = InitFastLED();
}

void Firmware::update()
{
    // readEncoder()
    // update global time
    // btnMode.update(g_time)
    // key_matrix.keyKeys()
}