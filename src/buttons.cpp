#include <Arduino.h>

#include <buttons.h>
#include <actions.h>
#include <pins.h>

Button::Button(int pin) : _pin(pin), _startTime(0), _isPressed(false) {}

void Button::setup() {
    pinMode(_pin, INPUT_PULLUP);
}

unsigned long Button::get_duration() {
    bool isPressedNow = (digitalRead(_pin) == 0); 
    
    if (isPressedNow && !_isPressed) {
        _startTime = millis();
        _isPressed = true;
        return 0;
    }
    
    if (!isPressedNow && _isPressed) {
        _isPressed = false;
        return millis() - _startTime; 
    }
    
    return 0; 
}

void Button::check() {
    unsigned long duration = get_duration();
    if (duration > 50) handleButtonAction(_pin, duration);
}

Button buttonTop(BTN_TOP);
Button buttonMid(BTN_MID);
Button buttonBottom(BTN_BOTTOM);