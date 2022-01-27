#ifndef __FIRMWARE_H__
#define __FIRMWARE_H__

#include <Arduino.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <Timer.h>

#include "modules/led/led.h"
#include "modules/mykeypad/mykeypad.h"

namespace Firmware
{
    ////
    // Constants
    ////

#pragma region Constants

    const uint8_t PIN_ANALOG = A1;

#pragma endregion

    ////
    // Structure definitions
    ////

#pragma region Structures

#pragma endregion

    Encoder leftEncoder(10, 16); // TODO: Magic numbers!
    // Encoder rightEncoder(14, 15);

    unsigned long g_Time;
    unsigned long g_DeltaTime;
    void updateGlobalTime(unsigned long milliseconds);

    void onPhysicalKeyPress(KeypadEvent key);

    /*
 * Init functions
 */

    void InitSerial();

    long left_encoder_value = -999;

    void readEncoder()
    {
        long position = leftEncoder.read() / 4;
        if (position != left_encoder_value)
        {
            if (position > left_encoder_value)
            {
                Consumer.write(MEDIA_VOLUME_UP);
            }
            else
            {
                Consumer.write(MEDIA_VOLUME_DOWN);
            }

            left_encoder_value = position;
        }
    }

    void init();
    void update();
}

#endif // __FIRMWARE_H__