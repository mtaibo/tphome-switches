#include "network.h"
#include "actions.h"
#include "config.h"
#include "pins.h"

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

void set_position(float next_position) {

    if (abs(next_position - config.current_position) < 0.5) return;

    if (next_position == config.current_position) return;
    config.next_position =  next_position;

    if (next_position > config.current_position && config.active_relay != RELAY_UP) move_blind(UP);
    else if (next_position < config.current_position && config.active_relay != RELAY_DOWN) move_blind(DOWN);
}

void move_blind(Direction direction) {

    unsigned long now = millis();

    // Check if moving its needed
    if (config.is_moving || direction == STOP || config.is_waiting) {

        // Check if blind is already moving on same direction
        if (config.active_relay == RELAY_UP && direction == UP) return;
        if (config.active_relay == RELAY_DOWN && direction == DOWN) return;

        // Stop every led and relay
        digitalWrite(RELAY_UP, LOW);
        digitalWrite(RELAY_DOWN, LOW);
        digitalWrite(LED_TOP, LOW);
        digitalWrite(LED_BOTTOM, LOW);

        // Message published to the state topic when relays stop
        client.publish(config.state_topic, "STOP");

        // Change active relay and active leds to -1
        config.active_led = -1;
        config.active_relay = -1;

        // Adjust time and config to stop the blind
        config.stop_time = now;

        config.is_moving = false;
        config.is_waiting = false;

        // If the order was just to STOP, finish the action
        if (direction == STOP) {
            config.stop_led_time = now;
            config.pause_control = true;

            if (config.current_position >= 100.0) config.current_position = 100.0;
            else if (config.current_position <= 0.0) config.current_position = 0.0;

            // Round the current position
            config.current_position = (float)(int)(config.current_position + 0.5f);

            digitalWrite(LED_MID, HIGH); 
            return;
        }
    }

    // Check if blind is already at its lowest or highest position
    if (config.current_position <= 0.0 && direction == DOWN) return;
    if (config.current_position >= 100.0 && direction == UP) return;

    // Set the next led and relay to pending, to be turned on the next cycle
    config.pending_led = (direction == UP) ? LED_TOP : LED_BOTTOM;
    config.pending_relay = (direction == UP) ? RELAY_UP : RELAY_DOWN;

    config.is_waiting = true;

    // Control the stop_time to avoid the motor_safe_time when
    // the blind has not been moving for that time before 
    if (now - config.stop_time > config.motor_safe_time) {
        config.stop_time = now - config.motor_safe_time;
    }
}

void update_actions() {

    // Variable to calculate time limit exceed
    static unsigned long limit_reached_start_time = 0;

    unsigned long now = millis();
    unsigned long time_running = now - config.stop_time;

    // Control the is_waiting state, when any relay or led is waiting
    // to be active, this segment turn the other relay and led down to
    // turn the pending ones to the HIGH state and also publish the new 
    // state to the MQTT server. Also modifies execution data to the 
    // code segment that stops this relay by position or time
    if (config.is_waiting && (time_running >= config.motor_safe_time)) {

        // First of all, before setting any relay to HIGH, turn the other 
        // relay to LOW to prevent the motor of the collapse
        if (config.pending_relay == RELAY_UP) {
            digitalWrite(LED_BOTTOM, LOW);
            digitalWrite(RELAY_DOWN, LOW);
        } else if (config.pending_relay == RELAY_DOWN) {
            digitalWrite(LED_TOP, LOW);
            digitalWrite(RELAY_UP, LOW);
        }

        // Turn mid button led to LOW before turning any other led to HIGH
        digitalWrite(LED_MID, LOW);

        // Turn pending led and relay to HIGH and
        // set them as the active ones
        digitalWrite(config.pending_led, HIGH);
        digitalWrite(config.pending_relay, HIGH);
        config.active_led = config.pending_led;
        config.active_relay = config.pending_relay;

        // Publish message to the state topic when going up or down
        if (config.active_relay == RELAY_UP) client.publish(config.state_topic, "UP");
        else if (config.active_relay == RELAY_DOWN) client.publish(config.state_topic, "DOWN");

        // Configure the variables for the update actions function
        config.is_moving = true;
        config.is_waiting = false;

        limit_reached_start_time = 0;

        config.stop_time = now; 
        config.current_time_limit = (config.active_relay == RELAY_UP) ? config.up_time : config.down_time;
        config.last_cycle_time = now;
    }

    // Control the is_moving state, when the blind is moving, 
    // this segment of code will simply stop it and recalculate the current position
    // of the blind depending on where the movement is going
    if (config.is_moving) {

        // Calculate the time difference and update the 
        // last_cycle_time for the next delta calculation
        unsigned long delta = now - config.last_cycle_time; 
        config.last_cycle_time = now;

        // Calculate the step depending on the current_time_limit and
        // add that step or substract that step depending on that limit
        float step = (delta * 100.0) / config.current_time_limit;
        config.current_position += (config.active_relay == RELAY_UP) ? step : -step;

        // Prevent the current_position to get above 100 or below 0
        if (config.current_position > 100.0) config.current_position = 100.0;
        if (config.current_position < 0.0) config.current_position = 0.0;

        // If we are moving, and we get to the final position that this movement was trying to reach
        // just stop the movement with the move_blind(STOP) function
        if (config.next_position < 100.0 && config.next_position > 0.0) {
            if ((config.active_relay == RELAY_UP && config.current_position >= config.next_position) ||
                (config.active_relay == RELAY_DOWN && config.current_position <= config.next_position)) move_blind(STOP);}

        // When next_position is 100.0 or 0.0, calculate an exceed of 2 seconds 
        // from the current_time_limit to prevent the blind from getting at an
        // intermediate position when trying to reach limit
        else if ((config.current_position >= 100.0 && config.next_position == 100.0) || 
            (config.current_position <= 0.0 && config.next_position == 0.0)) { 
            if (limit_reached_start_time == 0) limit_reached_start_time = now;
            if (now - limit_reached_start_time >= 3000) {
                limit_reached_start_time = 0;
                move_blind(STOP);}
        } else limit_reached_start_time = 0;

        // If the time that the relay has been running is greater than the up_time 
        // or the down_time, therefore, the current_limit, the blind stops
        if (time_running >= config.current_time_limit+5000) move_blind(STOP);
    }

    // Code to control pause button
    if (config.pause_control && ((time_running >= config.mid_led_time) || config.is_moving)) {
        digitalWrite(LED_MID, LOW);
        config.pause_control = false;
    }

    // Code to control blinking
    if (config.is_blinking && millis() - config.last_blink >= config.blink_time) 
        blink(config.blinking_led, 1);
}

void handle_button_action(int pin, unsigned long duration) {

    if (pin == BTN_TOP) {
        if (duration < config.short_pulse) set_position(100);
        else if (duration > config.short_pulse && duration < config.long_pulse) {
          if (config.current_position < config.down_position) set_position(config.down_position);
          else if (config.current_position >= config.down_position) set_position(100);
        }
        else if (duration > config.long_pulse);
    } 
    
    else if (pin == BTN_MID) {
        if (duration < config.short_pulse) move_blind(STOP);
        else if (duration > config.short_pulse && duration < config.long_pulse) save_config();
        else if (duration > config.long_pulse) reset_memory();
    } 
    
    else if (pin == BTN_BOTTOM) {
      if (duration < config.short_pulse) {
          if (config.current_position > config.down_position) set_position(config.down_position);
          else if (config.current_position <= config.down_position) set_position(0);
      }
      else if (duration > config.short_pulse && duration < config.long_pulse) set_position(0);
      else if (duration > config.long_pulse);
    }
}
