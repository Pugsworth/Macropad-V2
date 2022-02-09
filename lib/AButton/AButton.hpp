#ifndef __ABUTTON_H__
#define __ABUTTON_H__

// #include <cstdlib>
// #include <Arduino.h>
#include <avr/common.h>


enum class DebounceState
{
    Idle,
    OnDebounce,
    On,
    OffDebounce,
    Off
};

enum class ButtonState
{
    Unknown,
    Pressed,
    Unpressed,
    Hold
};

#define HIGH 0x1
#define LOW 0x0

typedef uint8_t (*PinReadFunc)(uint8_t pinNumber);
typedef void (*PinModeFunc)(uint8_t pinNumber);
typedef void (*StateChangeCallback)(ButtonState state);
typedef void (*ActionCallback)(void);

// Platform agnostic button debouncing
// has no references to time counting functions or pin reading function. These must be supplied.
// time is assumed to be of type unsigned long
// pin read values assumed to be either 0x0 or 0x1 of type uint8_t
class AButton
{
private:
    PinReadFunc m_pinReadFunc = nullptr;
    PinModeFunc m_pinModeFunc = nullptr;
    StateChangeCallback m_stateChangeCallback = nullptr;
    ActionCallback m_tapCallback = nullptr;
    ActionCallback m_holdCallback = nullptr;


    ButtonState m_buttonState = ButtonState::Unknown;
    DebounceState m_debounceState = DebounceState::Idle;

    bool m_bJustPressed = false;
    uint8_t m_iHoldTime = 250;        // time in ms for the button to register as being held
    uint8_t m_iDebounceTimeLimit = 2; // Amount of time in ms that no changes must occur to move to a stable state
    long m_lastChangeTime = 0;        // The last time a change has occurred. This is used to determine if we are currently still bouncing or stable.
    uint8_t m_pin = 0;

    // A button "press" occurs when the button is fully depressed and undepressed
    // A button "hold" occurs when the button is held down after becoming stable for at least m_iHoldTime

    // private methods
    uint8_t callPinReadFunc(); // convienence method to call the PinReadFunc.
    void callPinModeFunc(); // convienence method to call the PinModeFunc.
    void callTapCallback();
    void callHoldCallback();
    void onButtonStateChange(ButtonState state);
    void updateStateMachine(uint8_t value, unsigned long time);

public:
    AButton() = default;
    AButton(const uint8_t pinNumber);
    AButton(const uint8_t pinNumber, PinReadFunc func);

    void update(unsigned long time); // works best when called at a fixed interval
    bool isDown(); // if the buttonstate is on; registered before bounce is stable
    bool isHeld(); // if the button is actively being held
    bool hasPressed(); // when the button was pressed on this update

    void setPinReadFunc(PinReadFunc func);
    void setPinModeFunc(PinModeFunc func);
    void setStateChangeCallback(StateChangeCallback state);
    void setTapCallback(ActionCallback callback); // called when a complete "tap" is completed
    void setHoldCallback(ActionCallback callback); // called when a hold is registered
};

#endif // __ABUTTON_H__