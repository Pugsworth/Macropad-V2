#include <Arduino.h>
#include <FastLED.h>

#include <HID-Project.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

#include <Keypad.h>

//
// My Includes
//
#include "firmware.h"
#include <Timer.h>
#include <AButton.h>
// #include <TimeKeeper.h>
#include "ChangeWithDeadzone.h"
#include "KeyBind.hpp"
#include "util.h"

///////////////////////////////////////
// 
///////////////////////////////////////

// TODO: Clean up everything just like the ESPBME code

//
// Macros
//
#pragma region Macros
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
#pragma endregion

//
// Constants
//
#pragma region Constants
const uint8_t MAX_LAYERS = 10;
const uint8_t ROWS = 3;
const uint8_t COLS = 4;

const uint8_t PIN_ANALOG = A1;
const uint8_t PIN_LED_DATA = A2;
const uint8_t LED_COUNT = 13;
#pragma endregion


//
// Objects
//
#pragma region Objects
CRGB leds[LED_COUNT] = {};

Encoder leftEncoder(10, 16);
// Encoder rightEncoder(14, 15);

AButton btnMode(A0, [](uint8_t pin) -> uint8_t { return digitalRead(pin); });

// TimeKeeper time_keeper();
unsigned long g_time = 0;
unsigned long g_deltaTime = 0;
#pragma endregion


//
// Keyboard Matrix Definitions
//
#pragma region Keyboard Matrix Definitions
char keys[ROWS][COLS] = {
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', 'A', 'B'}
};
KeyBind keymap[MAX_LAYERS][ROWS*COLS] = {
  { // Red
    MEDIA_VOLUME_MUTE, KEY_F13, KEY_F14, KEY_F15,
    KEY_F16,           KEY_F17, KEY_F18, KEY_F19,
    KEY_F20,           KEY_F21, KEY_F22, KEY_F23
  },
  { // Green
    MEDIA_VOLUME_MUTE, KEYPAD_7, KEYPAD_8, KEYPAD_9,
    KEYPAD_ADD,        KEYPAD_4, KEYPAD_5, KEYPAD_6,
    KEYPAD_0,          KEYPAD_1, KEYPAD_2, KEYPAD_3 
  },
  { // Blue
    MEDIA_VOLUME_MUTE,  KEY_HOME,       KEY_DELETE,      KEY_PAGE_UP,
    KEY_EQUAL,          KEY_END,        KEY_UP_ARROW,    KEY_PAGE_DOWN,
    KEY_MINUS,          KEY_LEFT_ARROW, KEY_DOWN_ARROW,  KEY_RIGHT_ARROW
  },
  { // Magenta
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
uint8_t layer_colors[MAX_LAYERS][3] = {
  {127, 0, 0},
  {0, 127, 0},
  {0, 0, 127},
  {127, 0, 127}
};
uint8_t rowPins[ROWS] = {4, 5, A3};
uint8_t colPins[COLS] = {6, 7, 8, 9};

uint8_t layer = 0;

Keypad key_matrix = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

KeyBind media_keys[4] { MEDIA_VOLUME_MUTE, MEDIA_PREVIOUS, MEDIA_PLAY_PAUSE, MEDIA_NEXT };
// When a key is pressed, this function is invoked which will return the appropriate KeyBind
KeyBind getBindForKey(char keyHex)
{
  int value = hex2int(keyHex);

  if (btnMode.isHeld()){
    if (value < 4) {
      return media_keys[value];
    }
  }

  return keymap[layer][value];
}
#pragma endregion

//
// Functions
//
#pragma region Functions

long left_encoder_value = -999;

void readEncoder()
{
  long position = leftEncoder.read() / 4;
  if (position != left_encoder_value)
  {
    // Serial.print("Encoder: ");
    // Serial.println(position);

    if (position > left_encoder_value) {
      Consumer.write(MEDIA_VOLUME_UP);
    } else {
      Consumer.write(MEDIA_VOLUME_DOWN);
    }

    left_encoder_value = position;
  }
}

void changeGlobalLEDColor(uint16_t value)
{
  uint8_t hue = map(value, 0, 1024, 0, 256) - 1;

  Serial.print("Changing hue to: ");
  Serial.println(hue);

  CRGB color;
  color.setHSV(hue, 255, 255);

  setall(leds, LED_COUNT, color.r, color.g, color.b);
}

void updateLEDs()
{
  // static int i = 0;
  // static int direction = 1;
  // static int hue = 0;

  // hue %= 256;
  // i %= (int)LED_COUNT;

  // Serial.println(i);

  // leds[i] = CHSV(hue++, 255, 255);
  // FastLED.show();
  // fadeall();

  // i += direction;

  // if (i == LED_COUNT && direction == 1) {
  //   direction = -1;
  // } else if (i == 0 && direction == -1) {
  //   direction = 1;
  // }

  // FastLED.delay(10);


  static int hue = 0;
  static int i = 0;
  static unsigned long nextHueChange = millis() + 30;
  static unsigned long nextIncrement = millis() + 10;
  static unsigned long nextFade = millis() + 3;

  if (millis() > nextHueChange) {
    hue = (hue + 1) % 256;
    nextHueChange = millis() + 10;
  }

  if (millis() > nextIncrement) {
    i = (i + 1) % LED_COUNT;
    nextIncrement = millis() + 100;

    leds[i] = CHSV(hue, 255, 255);
  }

  if (millis() > nextFade) {
    fadeall(leds, LED_COUNT);
    nextFade = millis() + 10;
  }
}

uint16_t analog_deadzone = 8;
ChangeWithDeadzone cwd(analog_deadzone, 0, 1023);
long last = 0;
long nextUpdate = 0;
long lastUpdate = 0;
long slowUpdateRate = 250;
long fastUpdateRate = 10;

enum class UPDATE_STATE : int {
  SLOW,
  SLOW_WAITING,
  FAST,
  FAST_WAITING
};

UPDATE_STATE state = UPDATE_STATE::SLOW;

void readAnalogDevice()
{
  unsigned long t = millis();

  if (nextUpdate < t)
  {
    uint16_t a = analogRead(PIN_ANALOG);
    long diff = last - a;
    long adiff = abs(diff);

    if (abs(diff) > 0) {
      last = a;
      Serial.print("Analog Difference:");
      Serial.println(diff);
    }

    switch (state)
    {
      case UPDATE_STATE::SLOW:
        nextUpdate = t + slowUpdateRate;
        break;

      case UPDATE_STATE::SLOW_WAITING:
        if (adiff > 4)
          state = UPDATE_STATE::FAST;
        break;

      case UPDATE_STATE::FAST:
        nextUpdate = t + fastUpdateRate;
        break;

      case UPDATE_STATE::FAST_WAITING:
        if (adiff > 4)
          state = UPDATE_STATE::SLOW;
        if (t - lastUpdate > 1000)
          state = UPDATE_STATE::SLOW_WAITING;
        break;
    }
  }

/*
  cwd.update(a);

  if (cwd.hasUpdate()) // if there is a change to the value beyond the deadzone
  {
    auto value = cwd.Value;
    Serial.print("readAnalogDevice cwd.hasUpdate():");
    Serial.println(value);

    int b = map(value, 0, 1023, 0, 255);
    FastLED.setBrightness(b);
    FastLED.show(b);
  }

*/
}

// Updates the g_deltaTime variable for timing functions
void updateTime()
{
  unsigned long g_deltaTime = millis() - g_time;
  g_time = millis();
}
#pragma endregion

//
// Arduino Functions
//
#pragma region Arduino Functions
void setup()
{
  // while (!Serial) {}

  InitSerial();
  InitKeyboards();

    // pinMode(PIN_ANALOG, INPUT);

    btnMode.setTapCallback([](){
        Serial.println("btnMode was tapped!!");
    });

  /*
  btnMode.attachClick([] {
    layer = (layer + 1) % MAX_LAYERS;
    auto steps = 1024/MAX_LAYERS;
    changeGlobalLEDColor(steps * layer);

    // auto col = layer_colors[layer];
    // setall(leds, LED_COUNT, col[0], col[1], col[2]);
  });
  */

  key_matrix.addEventListener([](KeypadEvent key) {
    if (key_matrix.getState() == PRESSED) {

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
        if (is_multi) {
          for (uint8_t i = 0; i < keybind.length; i++) {
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

  // TODO: Extract to InitFastLED
  FastLEDSetupData_t fastLEDData = {
    PinData = PIN_LED_DATA,
    LedCount = LED_COUNT,
    Leds = leds,
  };
  InitFastLED(fastLEDData);

  // setall(leds, LED_COUNT, 96, 0, 0);
  changeGlobalLEDColor(0);
}

void loop()
{
  // time_keeper.tick(millis());
  updateGlobalTime(millis());
  // update 
  readEncoder();
  btnMode.update(g_time);
  key_matrix.getKeys(); // necessary to update the matrix

  readAnalogDevice();

  cwd.tick(g_time);

  // cli();
  // updateLEDs();
  // sei();

  // FastLED.delay(1);
}
#pragma endregion