#include <Arduino.h>

template <uint8_t _pin>

class OutputPin {
  public:
    static void setup() {pinMode(_pin, OUTPUT); off();}
    static inline void on() {digitalWrite(_pin, HIGH);}
    static inline void off() {digitalWrite(_pin, LOW);}
    static void toggle() {digitalWrite(_pin, !digitalRead(_pin));}
    static inline void deactivate() {pinMode(_pin, INPUT);}
};
