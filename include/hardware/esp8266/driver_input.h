#include <Arduino.h>

template <uint8_t _pin, uint8_t _activeLevel = LOW, uint32_t _debounceMs = 50>

class InputPin {

  private:
    static inline bool _lastStableState;
    static inline bool _lastRawState;
    static inline uint32_t _lastDebounceTime;
    static inline uint32_t _pressStartTime;

  public:
    static void setup() {
        if (_activeLevel == LOW) {
            pinMode(_pin, INPUT_PULLUP);
        } else {
            pinMode(_pin, INPUT);
        }
        _lastStableState  = false;
        _lastRawState     = _rawRead();
        _lastDebounceTime = 0;
    }

    static bool isPressed() {
        bool currentRaw = (digitalRead(_pin) == _activeLevel);
        uint32_t now    = millis();

        if (currentRaw != _lastRawState) {
            _lastDebounceTime = now;
        }

        if ((now - _lastDebounceTime) > _debounceMs) {
            if (currentRaw != _lastStableState) {
                _lastStableState = currentRaw;
                if (_lastStableState)
                    _pressStartTime = now;
            }
        }
        _lastRawState = currentRaw;
        return _lastStableState;
    }

    static uint32_t getPressDuration() {
        if (!isPressed())
            return 0;
        return millis() - _pressStartTime;
    }
};
