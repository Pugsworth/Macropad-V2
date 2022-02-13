#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <Keypad.h>

// #define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

#include "KeyBind.hpp"
#include "AButton.hpp"
#include "Callback.hpp"


namespace MyKeypad
{
    // void onPhysicalKeyPress(KeypadEvent key);

    static const uint8_t PIN_ANALOG = A1;
    static const uint8_t MAX_LAYERS = 8;
    static const uint8_t ROWS = 3;
    static const uint8_t COLS = 4;

    // These should really be const, but the KeyPad constructor doesn't accept that
    extern uint8_t rowPins[ROWS];
    extern uint8_t colPins[COLS];

    extern Callback layerChange;
    extern uint8_t layer;

    // When a physical button is pressed, the keymap sends one of these corrisponding to which was pressed
    // a better way would be to just send an unsigned int, but this works for now...
    // TODO: Look into modifying the KeyPad library
    extern char keys[ROWS][COLS];

    extern AButton btnMode;

    extern KeyBind keymap[MAX_LAYERS][ROWS*COLS];

    extern Keypad key_matrix;

    // These are the media keys sent when pressing key 0-3 while holding the mode button
    extern KeyBind media_keys[4];

    extern long left_encoder_value;
    extern Encoder leftEncoder;
    //extern Encoder rightEncoder;

    // When a key is pressed, this function is invoked which will return the appropriate KeyBind
    KeyBind getBindForKey(char keyHex);

    void readEncoder();

    void onLayerChange(uint8_t layer);

    void InitKeyboard();

    void Update(unsigned long time);
}

#endif // __KEYPAD_H__