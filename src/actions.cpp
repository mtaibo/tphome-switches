#include <actions.h>
#include <config.h>
#include <pins.h>

int pause_control = false;
unsigned long mid_stop_time;

int blinkCount = 0;
int blinkingLed = LED_GREEN;
bool blinkingLedState = false;
unsigned long nextBlinkTime = 0;

void startBlink(int led) {
    blinkCount = 20;
    nextBlinkTime = millis();
}

void blindUp() {
    if (config.is_moving) blindStop();

    digitalWrite(LED_TOP, HIGH);
    
    config.is_waiting = true;
    config.motor_safe_time = millis() + 500; 
    config.pending_relay = RELAY_UP;
    
    config.active_led = LED_TOP;
}

void blindStop() {
    digitalWrite(config.active_relay, LOW);
    digitalWrite(config.active_led, LOW);

    if (!config.is_moving) digitalWrite(LED_MID, HIGH);
    mid_stop_time = millis() + 1000;
    pause_control = true;
    config.is_moving = false;
}

void blindDown() {
    if (config.is_moving) blindStop();

    digitalWrite(LED_BOTTOM, HIGH);
    
    config.is_waiting = true;
    config.motor_safe_time = millis() + 500; 
    config.pending_relay = RELAY_DOWN;
    
    config.active_led = LED_BOTTOM;
}

void updateActions() {
    
    if (config.is_waiting && millis() >= config.motor_safe_time) {
        digitalWrite(config.pending_relay, HIGH);
        config.is_moving = true;
        config.is_waiting = false;
        
        if (config.pending_relay == RELAY_UP) {
            config.stop_time = millis() + config.up_time;
            config.active_relay = RELAY_UP;
        } else {
            config.stop_time = millis() + config.down_time;
            config.active_relay = RELAY_DOWN;
        }
    }

    if (config.is_moving && millis() >= config.stop_time) {
        blindStop();
    }

    // Code to control pause button
    if (pause_control == true && millis() >= mid_stop_time) {
        digitalWrite(LED_MID, LOW);
        pause_control = false;
    }

    // Code to control blinking
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

    if (pin == BTN_TOP) {
        if (duration < config.short_pulse) blindUp();
        else if (duration > config.short_pulse && duration < config.long_pulse) startBlink(LED_TOP);
        else if (duration > config.long_pulse) ;
    } 
    
    else if (pin == BTN_MID) {
        if (duration < config.short_pulse) blindStop();
        else if (duration > config.short_pulse && duration < config.long_pulse) startBlink(LED_TOP);
        else if (duration > config.long_pulse) ;
    } 
    
    else if (pin == BTN_BOTTOM) {
        if (duration < config.short_pulse) blindDown();
        else if (duration > config.short_pulse && duration < config.long_pulse) startBlink(LED_TOP);
        else if (duration > config.long_pulse) ;
    }
}