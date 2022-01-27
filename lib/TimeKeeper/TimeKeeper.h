class TimeKeeper
{
private:
    unsigned long m_time = 0;
    unsigned long m_deltaTime = 0;
public:
    TimeKeeper() = default;

    void tick(unsigned long new_time)
    {
        m_deltaTime = new_time - m_time;
        m_time += m_deltaTime;
    }

    unsigned long getDeltaTime()
    {
        return m_deltaTime;
    }

    unsigned long getTime()
    {
        return m_time;
    }
};