#include <HID-Project.h>

typedef struct _KeyBind_t
{
    uint8_t type = 0;
    uint8_t length = 0;
    KeyboardKeycode key;
    KeyboardKeycode modifiers[3];

    _KeyBind_t(KeyboardKeycode key) :
        type(0),
        length(0),
        key(key)
    {}
    _KeyBind_t(ConsumerKeycode key) :
        type(1),
        length(0),
        key((KeyboardKeycode)key)
    {}

    _KeyBind_t(uint16_t key) :
        type(0),
        length(0),
        key((KeyboardKeycode)key)
    {}


    _KeyBind_t(KeyboardKeycode mod, KeyboardKeycode key) :
        type(0),
        length(1),
        key(key),
        modifiers{mod}
    {}
    _KeyBind_t(KeyboardKeycode mod, ConsumerKeycode key) :
        type(1),
        length(1),
        key((KeyboardKeycode)key),
        modifiers{mod}
    {}

    _KeyBind_t(KeyboardKeycode mod1, KeyboardKeycode mod2, KeyboardKeycode key) :
        type(0),
        length(2),
        key(key),
        modifiers{mod1, mod2}
    {}
    _KeyBind_t(KeyboardKeycode mod1, KeyboardKeycode mod2, KeyboardKeycode mod3, KeyboardKeycode key) :
        type(0),
        length(3),
        key(key),
        modifiers{mod1, mod2, mod3}
    {}
} KeyBind;