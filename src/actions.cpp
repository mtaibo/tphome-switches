#include <actions.h>
#include <config.h>
#include <pins.h>

void blink(int pin, int control) {

    // Stop control
    if (control == 0) {
        config.is_blinking = false;
        config.blinking_led = -1;
        digitalWrite(pin, LOW);
        return; 
    }

    // Control blinking, for the first time and the following times
    if (control == 1) {

        // If its the first blink, setup the config variables, 
        // otherwise, change blinking_state
        if (!config.is_blinking) {
            config.is_blinking = true;
            config.blinking_led = pin;
            config.blinking_state = HIGH;
        } else config.blinking_state = !config.blinking_state;

        // Then write on pin the state, high or low, and set the last_blink time
        digitalWrite(pin, config.blinking_state);
        config.last_blink = millis();
    }
}

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
        config.stop_led_time = millis();
        config.stop_time = millis();

        config.pause_control = true;
        config.is_moving = false;
        config.is_waiting = false;

        // If the order was just to STOP, finish the action
        if (direction == STOP) {digitalWrite(LED_MID, HIGH); return;}
    } 
    
    else if (!config.is_waiting) {
        
        // Check if blind is already at its lowest or highest position
        if (config.current_position <= 0.0 && direction == DOWN) return;
        if (config.current_position >= 100.0 && direction == UP) return;
        
        // Set the next led and relay to be turned on
        config.pending_led = (direction == UP) ? LED_TOP : LED_BOTTOM;
        config.pending_relay = (direction == UP) ? RELAY_UP : RELAY_DOWN;

        config.stop_time = millis();
        config.is_waiting = true;
    }
}

void updateActions() {

    unsigned long time_running = millis() - config.stop_time;

    // Control waiting and pending status for relays
    if (config.is_waiting && (time_running >= config.motor_safe_time)) {

        config.active_led = config.pending_led;
        config.active_relay = config.pending_relay;

        digitalWrite(config.active_led, HIGH);
        digitalWrite(config.active_relay, HIGH);
        
        config.stop_time = millis(); 
        config.current_limit = (config.active_relay == RELAY_UP) ? config.up_time : config.down_time;
        
        config.is_moving = true;
        config.is_waiting = false;
    }

    // Control common moving with stop_time
    else if (config.is_moving && (time_running >= config.current_limit)) moveBlind(STOP);

    // Code to control pause button
    else if (config.pause_control && (time_running >= config.mid_led_time)) {
        digitalWrite(LED_MID, LOW);
        config.pause_control = false;
    }

    // Code to control blinking
    if (config.is_blinking && millis() - config.last_blink >= 500) blink(config.blinking_led, 1);
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