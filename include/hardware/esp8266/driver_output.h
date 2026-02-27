#include <Arduino.h>

template <uint8_t _pin, uint8_t _onState>

class OutputPin {

  public:
    static void setup() {
        pinMode(_pin, OUTPUT);
        off();
    }
    static inline void on() { digitalWrite(_pin, _onState); }
    static inline void off() {
        digitalWrite(_pin, (_onState == HIGH) ? LOW : HIGH);
    }
    static void toggle() { digitalWrite(_pin, !digitalRead(_pin)); }
    static inline void deactivate() { pinMode(_pin, INPUT); }
};
