#include <actions.h>
#include <config.h>
#include <pins.h>

// An enum to simplify direction reading and to prevent errors
enum Direction { UP = 1, DOWN = 0, STOP = -1 };

int pause_control = false;

int blinkCount = 0;
int blinkingLed = LED_GREEN;
bool blinkingLedState = false;
unsigned long nextBlinkTime = 0;

void startBlink(int led) {
    blinkCount = 20;
    nextBlinkTime = millis();
}

void moveBlind(Direction direction) {

    // Check if moving its needed
    if (config.is_moving || direction == STOP) {

        // Check if blind is already moving on same direction
        if (config.active_relay == RELAY_UP && direction == 1) return;
        if (config.active_relay == RELAY_DOWN && direction == 0) return;
        
        // Stop every led and relay
        digitalWrite(RELAY_UP, LOW);
        digitalWrite(RELAY_DOWN, LOW);
        digitalWrite(LED_TOP, LOW);
        digitalWrite(LED_BOTTOM, LOW);

        // Change active relay and active leds to -1
        config.active_led = -1;
        config.active_relay = -1;

        // Adjust time and config to stop the blind
        // ! Take a look at this three lines below
        config.is_moving = false;
        config.stop_time = millis() + config.stop_led_time;
        pause_control = true;

        // If the order was just to STOP, finish the action
        if (direction == STOP) {digitalWrite(LED_MID, HIGH); return;}
    }

    // Check if blind is already at its lowest or highest position
    if (config.current_position == 0.0 && direction == 0) return;
    else if (config.current_position == 100.0 && direction == 1) return;
    
    config.active_led = (direction == UP) ? LED_TOP : LED_BOTTOM;
    config.pending_relay = (direction == UP) ? RELAY_UP : RELAY_DOWN;
    
    digitalWrite(config.active_led, HIGH);

    // Adjust time and config to close the blind
    // ! Take a look at this two lines
    config.motor_safe_time = millis() + 500; 
    config.is_waiting = true;
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
    if (pause_control == true && millis() >= config.stop_time) {
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
        if (duration < config.short_pulse) moveBlind(UP);
        else if (duration > config.short_pulse && duration < config.long_pulse) startBlink(LED_TOP);
        else if (duration > config.long_pulse) ;
    } 
    
    else if (pin == BTN_MID) {
        if (duration < config.short_pulse) moveBlind(STOP);
        else if (duration > config.short_pulse && duration < config.long_pulse) startBlink(LED_MID);
        else if (duration > config.long_pulse) ;
    } 
    
    else if (pin == BTN_BOTTOM) {
        if (duration < config.short_pulse) moveBlind(DOWN); 
        else if (duration > config.short_pulse && duration < config.long_pulse) startBlink(LED_BOTTOM);
        else if (duration > config.long_pulse) ;
    }
}