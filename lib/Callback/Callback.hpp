#ifndef __CALLBACK_H__
#define __CALLBACK_H__

#include <avr/common.h>


typedef void (*callback_func)(uint8_t data);

class Callback
{
private:
    callback_func* m_callbacks;
    uint8_t m_count;
    uint8_t m_lastAdded;

public:

    Callback() = default;
    Callback(uint8_t count);
    ~Callback();

    void operator+=(callback_func callback);

    void Invoke(uint8_t data);
};

#endif // __CALLBACK_H__