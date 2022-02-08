#include <stdlib.h>

class ChangeWithDeadzone
{
private:
    int m_minValue;
    int m_maxValue;
    int m_deadzone;
    bool m_valueChanged = false;
    unsigned long lastValueChange = 0;
    unsigned long m_timeout = 1000;
    unsigned long m_time = 0;

public:
    int Value = 0;

    ChangeWithDeadzone(int deadzone, int min_value, int max_value) : 
    m_minValue(min_value),
    m_maxValue(max_value), 
    m_deadzone(deadzone),
    Value(0)
    {}

    void update(int value)
    {
        if (m_valueChanged) {
            Value = value;
            lastValueChange = m_time;
        }
        else {

            int difference = abs(this->Value - value);
            if (difference >= m_deadzone) {
                // Do something about the change
                m_valueChanged = true;
                Value = value;
            }
        }
    }

    void tick(unsigned long time)
    {
        m_time = time;

        unsigned long time_passed = time - lastValueChange;

        if (time_passed >= m_timeout) {
            m_valueChanged = false;
        }
    }

    bool hasUpdate()
    {
        return m_valueChanged;
    }
};

class StateMachine
{
private:
    int m_stack[32] = {};
public:
    StateMachine();
};