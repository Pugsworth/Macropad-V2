class Timer
{
private:
    unsigned long m_updateRate = 1000;
    unsigned long m_time = 0;
public:

    Timer(long tickrate) : m_updateRate(tickrate) {}

    bool Elapsed(long ticksToAdd);
    void SetUpdateRate(long tickrate);
};