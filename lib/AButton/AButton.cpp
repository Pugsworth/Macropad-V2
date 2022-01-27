#include "AButton.h"

AButton::AButton(uint8_t pinNumber)
{
    m_pin = pinNumber;
}

AButton::AButton(uint8_t pinNumber, PinReadFunc func)
{
    m_pin = pinNumber;
    setPinReadFunc(func);
}

void AButton::update(unsigned long time)
{
    uint8_t value = callPinReadFunc();

    updateStateMachine(value, time);
}

void AButton::updateStateMachine(uint8_t value, unsigned long time)
{
    static uint8_t lastValue = 0;

    // regardless of Debounce State, this time should be updated whenever there is a change in value
    if (value != lastValue) {
        m_lastChangeTime = time;
    }

    switch (m_debounceState)
    {
    case DebounceState::Idle:
        m_debounceState = DebounceState::Off;
        break;

    case DebounceState::OnDebounce:
        if (m_bJustPressed)
            m_bJustPressed = false;

        if ((time - m_lastChangeTime) > m_iDebounceTimeLimit) {
            m_debounceState = DebounceState::On;
            break;
        }

        break;

    case DebounceState::On:
        if (value == HIGH) {
            m_debounceState = DebounceState::OffDebounce;

            if (m_buttonState != ButtonState::Hold) {
                // TODO: Register a full "click" right now
                // m_bJustPressed = true;
                callTapCallback();
            }
            break;
        }

        if (m_buttonState != ButtonState::Hold &&
            (time - m_lastChangeTime) > m_iHoldTime) {
            // m_buttonState = ButtonState::Hold;
            onButtonStateChange(ButtonState::Hold);
            callHoldCallback();
        }

        break;

    case DebounceState::OffDebounce:
        if ((time - m_lastChangeTime) > m_iDebounceTimeLimit) {
            m_debounceState = DebounceState::Off;
            // m_buttonState = ButtonState::Unpressed;
            onButtonStateChange(ButtonState::Unpressed);
            break;
        }

        break;

    case DebounceState::Off:
        if (value == LOW) {
            m_debounceState = DebounceState::OnDebounce;
            // m_buttonState = ButtonState::Pressed; // instant reaction on press, delayed reaction on unpress
            onButtonStateChange(ButtonState::Pressed);
            m_bJustPressed = true; // The button was just pressed down on this "frame"
        }

        break;
    }

    lastValue = value;
}

bool AButton::isDown()
{
    return m_buttonState == ButtonState::Pressed;
}

bool AButton::isHeld()
{
    return m_buttonState == ButtonState::Hold;
}

bool AButton::hasPressed()
{
    return m_bJustPressed;
}

void AButton::setPinReadFunc(PinReadFunc func)
{
    m_pinReadFunc = func;
}

uint8_t AButton::callPinReadFunc()
{
    if (m_pinReadFunc != nullptr) {
        return m_pinReadFunc(m_pin);
    }
}

void AButton::onButtonStateChange(ButtonState state)
{
    if (m_stateChangeCallback != nullptr)
        m_stateChangeCallback(state);

    m_buttonState = state;
}

void AButton::setStateChangeCallback(StateChangeCallback state)
{
    m_stateChangeCallback = state;
}

void AButton::callTapCallback()
{
    if (m_tapcallback != nullptr)
        m_tapCallback();
}

void AButton::setTapCallback(ActionCallback callback)
{
    m_tapCallback = callback;
}

void AButton::callHoldCallback()
{
    if (m_holdcallback != nullptr)
        m_holdcallback();
}

void AButton::setHoldCallback(ActionCallback callback)
{
    m_holdCallback = callback;
}