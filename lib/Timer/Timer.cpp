#include "Timer.hpp"

bool Timer::Elapsed(long ticksToAdd)
{
    m_time -= ticksToAdd;

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