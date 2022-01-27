#include "Timer.h"

bool Timer::Elapsed(long time)
{
    m_time -= time;

    if (m_time <= 0) {
        m_time = m_updateRate;
        return true;
    }

    return false;
}

void Timer::SetUpdateRate(long tickrate)
{
    m_updateRate = tickrate;
}