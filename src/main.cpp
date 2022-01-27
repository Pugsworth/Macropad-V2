#include "firmware.h"

// The simplest main.cpp file ever!

int main()
{
    Firmware::init();
}

void loop()
{
    Firmware::update();
}