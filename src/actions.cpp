#include "actions.h"
#include "config.h"
#include "pins.h"


unsigned long nextBlinkTime = 0;
int blinkCount = 0;
int blinkingLed = LED_GREEN;
bool blinkingLedState = false;

void startBlink(int led) {
    blinkCount = 20;
    nextBlinkTime = millis();
}

void updateActions() {
    if (config.is_moving && millis() >= config.stop_time) {
        blindStop();
    }

    if (blinkCount > 0 && millis() >= nextBlinkTime) {
        blinkingLedState = !blinkingLedState;
        digitalWrite(blinkingLed, blinkingLedState);
        blinkCount--;
        nextBlinkTime = millis() + 1000; 
        if (blinkCount == 0) {
            digitalWrite(blinkingLed, LOW); 
            blinkingLedState = false;            
        }
    }
}

void handleButtonAction(int pin, unsigned long duration) {
    if (duration == 0) return;

    if (pin == BTN_TOP) {
        if (duration < config.short_pulse) {
            blindUp();
        } 
    } 
    
    else if (pin == BTN_MID) {
        blindStop();

        if (duration > config.short_pulse) {
            startBlink(LED_GREEN); 
        }
    } 
    
    else if (pin == BTN_BOTTOM) {
        if (duration < config.short_pulse) {
            blindDown();
        }
    }
}

void blindUp() {
    if (config.is_moving) blindStop(); 

    digitalWrite(LED_TOP, HIGH);
    digitalWrite(RELAY_UP, HIGH);

    config.is_moving = true;
    config.stop_time = millis() + config.up_time; 

    config.active_relay = RELAY_UP;
    config.active_led = LED_TOP;
}

void blindDown() {
    if (config.is_moving) blindStop();

    digitalWrite(LED_BOTTOM, HIGH);
    digitalWrite(RELAY_DOWN, HIGH);

    config.is_moving = true;
    config.stop_time = millis() + config.down_time;

    config.active_relay = RELAY_DOWN;
    config.active_led = LED_BOTTOM;
}

void blindStop() {
    digitalWrite(config.active_relay, LOW);
    digitalWrite(config.active_led, LOW);
    
    config.is_moving = false;
}