#include "button.h"

Button::Button(int pin) : _pin(pin), _startTime(0), _isPressed(false) {}

void Button::begin() {
    pinMode(_pin, INPUT_PULLUP);
}

unsigned long Button::checkPulse() {
    bool currentState = (digitalRead(_pin) == 0); 
    
    if (currentState && !_isPressed) {
        _startTime = millis();
        _isPressed = true;
        return 0;
    }
    
    if (!currentState && _isPressed) {
        unsigned long duration = millis() - _startTime;
        _isPressed = false;
        return duration; 
    }
    
    return 0; 
}