#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <Keypad.h>
#include "KeyBind.hpp"
#include "AButton.h"


// The total number of layers can be determined with 1 + (2^n) - 1 where n = number of modifiers used in combination
// Total of 8 possible "layers" with combination of F13-F24 and modifier keys
// this combined with using the advanced features of AutoHotKey, this should be more than enough!
// One modifier
#define WITH_CTRL(key) {KEY_LEFT_CTRL, key}
#define WITH_SHIFT(key) {KEY_LEFT_SHIFT, key}
#define WITH_ALT(key) {KEY_LEFT_ALT, key}
// Two modifiers
#define WITH_CTRL_SHIFT(key) {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, key}
#define WITH_CTRL_ALT(key) {KEY_LEFT_CTRL, KEY_LEFT_ALT, key}
#define WITH_SHIFT_ALT(key) {KEY_LEFT_SHIFT, KEY_LEFT_ALT, key}
// Three modifiers
#define WITH_CTRL_SHIFT_ALT(key) {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, KEY_LEFT_ALT, key}


static const uint8_t MAX_LAYERS = 8;
static const uint8_t ROWS = 3; // TODO: Magic number!
static const uint8_t COLS = 4;

// These should really be const, but the KeyPad constructor doesn't accept that
uint8_t rowPins[ROWS] = {4, 5, A3};
uint8_t colPins[COLS] = {6, 7, 8, 9};

uint8_t layer = 0;

// When a physical button is pressed, the keymap sends one of these corrisponding to which was pressed
// a better way would be to just send an unsigned int, but this works for now...
// TODO: Look into modifying the KeyPad library
char keys[ROWS][COLS] = {
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', 'A', 'B'}
};

AButton btnMode(A0, [](uint8_t pin) -> uint8_t { return digitalRead(pin); });

KeyBind keymap[MAX_LAYERS][ROWS*COLS] = {
  { // Standard
    MEDIA_VOLUME_MUTE, KEY_F13, KEY_F14, KEY_F15,
    KEY_F16,           KEY_F17, KEY_F18, KEY_F19,
    KEY_F20,           KEY_F21, KEY_F22, KEY_F23
  },

  { // Ctrl
    MEDIA_VOLUME_MUTE,  WITH_CTRL(KEY_F13), WITH_CTRL(KEY_F14), WITH_CTRL(KEY_F15),
    WITH_CTRL(KEY_F16), WITH_CTRL(KEY_F17), WITH_CTRL(KEY_F18), WITH_CTRL(KEY_F19),
    WITH_CTRL(KEY_F20), WITH_CTRL(KEY_F21), WITH_CTRL(KEY_F22), WITH_CTRL(KEY_F23),
  },

  { // Shift
    MEDIA_VOLUME_MUTE,   WITH_SHIFT(KEY_F13), WITH_SHIFT(KEY_F14), WITH_SHIFT(KEY_F15),
    WITH_SHIFT(KEY_F16), WITH_SHIFT(KEY_F17), WITH_SHIFT(KEY_F18), WITH_SHIFT(KEY_F19),
    WITH_SHIFT(KEY_F20), WITH_SHIFT(KEY_F21), WITH_SHIFT(KEY_F22), WITH_SHIFT(KEY_F23)
  },

  { // Alt
    MEDIA_VOLUME_MUTE, WITH_ALT(KEY_F13), WITH_ALT(KEY_F14), WITH_ALT(KEY_F15),
    WITH_ALT(KEY_F16), WITH_ALT(KEY_F17), WITH_ALT(KEY_F18), WITH_ALT(KEY_F19),
    WITH_ALT(KEY_F20), WITH_ALT(KEY_F21), WITH_ALT(KEY_F22), WITH_ALT(KEY_F23)
  },

  { // Ctrl Shift
    MEDIA_VOLUME_MUTE,        WITH_CTRL_SHIFT(KEY_F13), WITH_CTRL_SHIFT(KEY_F14), WITH_CTRL_SHIFT(KEY_F15),
    WITH_CTRL_SHIFT(KEY_F16), WITH_CTRL_SHIFT(KEY_F17), WITH_CTRL_SHIFT(KEY_F18), WITH_CTRL_SHIFT(KEY_F19),
    WITH_CTRL_SHIFT(KEY_F20), WITH_CTRL_SHIFT(KEY_F21), WITH_CTRL_SHIFT(KEY_F22), WITH_CTRL_SHIFT(KEY_F23)
  },

  { // Ctrl Alt
    MEDIA_VOLUME_MUTE,      WITH_CTRL_ALT(KEY_F13), WITH_CTRL_ALT(KEY_F14), WITH_CTRL_ALT(KEY_F15),
    WITH_CTRL_ALT(KEY_F16), WITH_CTRL_ALT(KEY_F17), WITH_CTRL_ALT(KEY_F18), WITH_CTRL_ALT(KEY_F19),
    WITH_CTRL_ALT(KEY_F20), WITH_CTRL_ALT(KEY_F21), WITH_CTRL_ALT(KEY_F22), WITH_CTRL_ALT(KEY_F23)
  },

  { // Shift Alt
    MEDIA_VOLUME_MUTE,       WITH_SHIFT_ALT(KEY_F13), WITH_SHIFT_ALT(KEY_F14), WITH_SHIFT_ALT(KEY_F15),
    WITH_SHIFT_ALT(KEY_F16), WITH_SHIFT_ALT(KEY_F17), WITH_SHIFT_ALT(KEY_F18), WITH_SHIFT_ALT(KEY_F19),
    WITH_SHIFT_ALT(KEY_F20), WITH_SHIFT_ALT(KEY_F21), WITH_SHIFT_ALT(KEY_F22), WITH_SHIFT_ALT(KEY_F23)
  },

  { // Ctrl Shift Alt
    MEDIA_VOLUME_MUTE,            WITH_CTRL_SHIFT_ALT(KEY_F13), WITH_CTRL_SHIFT_ALT(KEY_F14), WITH_CTRL_SHIFT_ALT(KEY_F15),
    WITH_CTRL_SHIFT_ALT(KEY_F16), WITH_CTRL_SHIFT_ALT(KEY_F17), WITH_CTRL_SHIFT_ALT(KEY_F18), WITH_CTRL_SHIFT_ALT(KEY_F19),
    WITH_CTRL_SHIFT_ALT(KEY_F20), WITH_CTRL_SHIFT_ALT(KEY_F21), WITH_CTRL_SHIFT_ALT(KEY_F22), WITH_CTRL_SHIFT_ALT(KEY_F23)
  }
};


Keypad key_matrix = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// These are the media keys sent when pressing key 0-3 while holding the mode button
KeyBind media_keys[4] { MEDIA_VOLUME_MUTE, MEDIA_PREVIOUS, MEDIA_PLAY_PAUSE, MEDIA_NEXT };

// When a key is pressed, this function is invoked which will return the appropriate KeyBind
KeyBind getBindForKey(char keyHex);

void InitKeyboard()
{
    Consumer.begin();
    BootKeyboard.begin();

    key_matrix.addEventListener([](KeypadEvent key)
    {
        if (key_matrix.getState() == PRESSED)
        {
            Serial.print("Layer: ");
            Serial.println(layer);

            KeyBind keybind = getBindForKey(key);

            Serial.println(keybind.key);

            switch (key)
            {
                // This one is basically always the same
            case '0':
                Consumer.write(MEDIA_VOLUME_MUTE);
                break;

            default:
                bool is_multi = keybind.length > 0;
                if (is_multi)
                {
                    for (uint8_t i = 0; i < keybind.length; i++)
                    {
                        BootKeyboard.press(keybind.modifiers[i]);
                    }
                }

                BootKeyboard.write(keybind.key);

                if (is_multi)
                    BootKeyboard.releaseAll();

                break;
            }
        }
    });

    delay(1000);
}

#endif // __KEYPAD_H__