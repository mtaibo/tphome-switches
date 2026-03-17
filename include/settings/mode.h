#ifndef MODE_H
#define MODE_H

namespace Mode {

    enum Value {
        NORMAL,
        CONNECTION
    };

    inline Value current = CONNECTION;
    inline bool is(Value mode) { return current == mode; }
    inline void set(Value mode) { current = mode; }
}

#endif // MODE_H