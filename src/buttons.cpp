#include <Arduino.h>

#include "buttons.h"
#include "actions.h"
#include "config.h"
#include "pins.h"

Button::Button(int pin) : _pin(pin), _startTime(0), _wasPressed(false), _waitingRelease(false){}

void Button::setup() {
    pinMode(_pin, INPUT_PULLUP);
}

unsigned long Button::get_duration() {

    // Read pin to see if it is pressed now
    bool isPressed = (digitalRead(_pin) == LOW); 

    // If it is not pressed now, check if it was pressed or if it was a release
    // and retart the corresponding variables
    if (!isPressed && (_wasPressed || _waitingRelease)) {
        unsigned long duration = 0; // Needed local variable to restart _startTime 

        // If it was not waiting a release, it means the duration was not
        // sent before, so it calculates the duration from _startTime
        if (!_waitingRelease && _startTime > 0) {
            duration = millis() - _startTime;
        }

        // Otherwise, it was a release coming from a waitingRelease, so the duration
        // will be 0, and it stops being on _waitingRelease state
        _wasPressed = false;
        _waitingRelease = false;
        _startTime = 0;
        
        return duration;
    }

    // If it is pressed, and it was not pressed before, start the counter
    if (isPressed && !_wasPressed && !_waitingRelease) {
        _startTime = millis();
        _wasPressed = true;
        return 0;
    }

    // If the button was pressed, it is pressed now, and the time that it has
    // been pressed exceeds the long pulse, set the waiting release status
    if (isPressed && _wasPressed && (millis() - _startTime > config.long_pulse)) {
        _wasPressed = false; 
        _waitingRelease = true;  
        return millis() - _startTime;
    }

    return 0; 
}

// Main button function to handle actions depending on the pulse duration
void Button::check() {
    unsigned long duration = get_duration();
    if (duration > 50) handle_button_action(_pin, duration);
}

Button buttonTop(BTN_TOP);
Button buttonMid(BTN_MID);
Button buttonBottom(BTN_BOTTOM);