#include "Callback.hpp"


Callback::Callback(uint8_t count) : m_count(count), m_lastAdded(0)
{
    m_callbacks = new callback_func[count];
}
Callback::~Callback()
{
    delete[] m_callbacks;
}

void Callback::operator+=(callback_func callback)
{
    m_callbacks[m_lastAdded++] = callback;
}

void Callback::Invoke(uint8_t data)
{
    for (uint8_t i = 0; i < m_count; i++)
    {
        m_callbacks[i](data);
    }
    
}