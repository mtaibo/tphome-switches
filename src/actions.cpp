#include <actions.h>
#include <config.h>
#include <pins.h>

void moveBlind(Direction direction) {

    // Check if moving its needed
    if (config.is_moving || direction == STOP) {

        // Check if blind is already moving on same direction
        if (config.active_relay == RELAY_UP && direction == UP) return;
        if (config.active_relay == RELAY_DOWN && direction == DOWN) return;
        
        // Stop every led and relay
        digitalWrite(RELAY_UP, LOW);
        digitalWrite(RELAY_DOWN, LOW);
        digitalWrite(LED_TOP, LOW);
        digitalWrite(LED_BOTTOM, LOW);

        // Change active relay and active leds to -1
        config.active_led = -1;
        config.active_relay = -1;

        // Adjust time and config to stop the blind
        config.stop_led_time = millis() + config.mid_led_time;
        config.stop_time = millis() + config.motor_safe_time;
        config.pause_control = true;
        config.is_moving = false;
        config.is_waiting = false;

        // If the order was just to STOP, finish the action
        if (direction == STOP) {digitalWrite(LED_MID, HIGH); return;}
    }

    // Check if blind is already at its lowest or highest position
    if (config.current_position <= 0.0 && direction == DOWN) return;
    if (config.current_position >= 100.0 && direction == UP) return;
    
    // Set the next led and relay to be turned on
    config.pending_led = (direction == UP) ? LED_TOP : LED_BOTTOM;
    config.pending_relay = (direction == UP) ? RELAY_UP : RELAY_DOWN;

    config.is_waiting = true;
}

void updateActions() {

    // Control waiting and pending status for relays
    if (config.is_waiting && millis() >= config.stop_time) {

        config.active_led = config.pending_led;
        config.active_relay = config.pending_relay;

        digitalWrite(config.active_led, HIGH);
        digitalWrite(config.active_relay, HIGH);
        
        config.stop_time = (config.active_relay == RELAY_UP) ? millis() + config.up_time : millis() + config.down_time;
        
        config.is_moving = true;
        config.is_waiting = false;
    }

    // Control common moving with stop_time
    else if (config.is_moving && millis() >= config.stop_time) moveBlind(STOP);

    // Code to control pause button
    else if (config.pause_control && millis() >= config.stop_led_time) {
        digitalWrite(LED_MID, LOW);
        config.pause_control = false;
    }
}

void handleButtonAction(int pin, unsigned long duration) {

    if (pin == BTN_TOP) {
        if (duration < config.short_pulse) moveBlind(UP);
        else if (duration > config.short_pulse && duration < config.long_pulse) ;
        else if (duration > config.long_pulse) ;
    } 
    
    else if (pin == BTN_MID) {
        if (duration < config.short_pulse) moveBlind(STOP);
        else if (duration > config.short_pulse && duration < config.long_pulse) ;
        else if (duration > config.long_pulse) ;
    } 
    
    else if (pin == BTN_BOTTOM) {
        if (duration < config.short_pulse) moveBlind(DOWN); 
        else if (duration > config.short_pulse && duration < config.long_pulse) ;
        else if (duration > config.long_pulse) ;
    }
}