#include <Arduino.h>

template <uint8_t _pin, uint16_t _maxTime>

class InputPin {

  private:
    static inline bool _wasPressed = false;
    static inline bool _waitingRelease = false;
    static inline uint16_t _startTime = 0;

  public:
    static void setup() { pinMode(_pin, INPUT_PULLUP);}
    static inline bool isPressed() {return digitalRead(_pin) == LOW;}

    static uint16_t getDuration() {

        uint16_t now = (uint16_t) (millis() / 10);

        if (isPressed() && !_waitingRelease) {

            if (_wasPressed) {
                uint16_t duration = now - _startTime;
                if (duration > _maxTime) {
                    _waitingRelease = true; 
                    return duration;
                }

            } else { // Start timer
                _startTime = now;
                _wasPressed = true;
                _waitingRelease = false;
            }

        } else if (!isPressed() && _wasPressed){
            uint16_t duration = now - _startTime;
            _startTime = 0;
            _wasPressed = false;
            _waitingRelease = false;
            return duration;
        }

        return 0;
    }
};