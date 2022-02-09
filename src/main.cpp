#include <Arduino.h>
#include "firmware.hpp"
#include "TimeKeeper.hpp"

// The simplest main.cpp file ever!

TimeKeeper g_timekeeper;

void setup()
{
    Firmware::Init();
}

void loop()
{
    unsigned long t = millis();
    g_timekeeper.tick(t);
    Firmware::Update(g_timekeeper.getTime());
}