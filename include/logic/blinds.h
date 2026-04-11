#ifndef BLINDS_H
#define BLINDS_H

#include "settings.h"
#include "leds.h"

#if defined(DEVICE_HARDWARE_ESP8266)
    #include "hardware/esp8266/hardware.h"
#elif defined(DEVICE_HARDWARE_BK7231N)
    #include "hardware/bk7231n/hardware.h"
#endif

/* Forward declaration */
namespace Commands {
    void publishState();
}

namespace Blinds {

    static constexpr uint16_t TOL = 50; // Macro to compare different positions or times
    static constexpr uint32_t STOPPING_TIME = 2000; // 2.00s - Macro for time running after reaching max position

    enum State { IDLE, WAITING, MOVING, STOPPING };
    enum Direction { NONE, UP, DOWN };
        
    struct Motor {
        State state = IDLE;
        Direction direction = NONE;
        float realPosition = (float) Settings::state.currentPosition;
        uint32_t startTime = 0;
        uint32_t lastTime = 0;
        uint32_t waitingTime = 0;
        uint32_t statePublishTime = 0;
        uint16_t nextPosition = Settings::state.currentPosition;
    };
        
    static Motor _motor;

    namespace Relays {

        inline void stop() {

            /* Stop relays to prevent both relays 
             * being active simultaneously */
            Hardware::RelayUp::off();
            Hardware::RelayDown::off();

            /* Turn off every movement led to prevent
             * both leds being active simultaneously */
            Leds::set(Pins::LED_TOP, Leds::OFF);
            Leds::set(Pins::LED_BTM, Leds::OFF);

            /* Save settings if there is not more movement */
            if (_motor.state == IDLE) Settings::saveState();
        }

        inline void up() {
            Relays::stop(); // Stop movement before starting movement again

            /* Start next movement */
            Settings::prefs.invertedRelays ? Hardware::RelayDown::on() : Hardware::RelayUp::on();
            Leds::set(Pins::LED_TOP, Leds::ON);
        }

        inline void down() {
            Relays::stop(); // Stop movement before starting movement again

            /* Start next movement */
            Settings::prefs.invertedRelays ? Hardware::RelayUp::on() : Hardware::RelayDown::on();
            Leds::set(Pins::LED_BTM, Leds::ON);
        }
    }

    inline void stop() { // Blinds global action
        if (_motor.state != IDLE) {
            _motor.state = IDLE;
            _motor.direction = NONE;
            _motor.startTime = 0;
            _motor.realPosition = (float) Settings::state.currentPosition;

            if (_motor.nextPosition > Settings::state.currentPosition)
                if ((_motor.nextPosition - Settings::state.currentPosition) < TOL)
                    Settings::state.currentPosition = _motor.nextPosition;
            
            Relays::stop();
            Leds::set(Pins::LED_MID, Leds::OFF);
            Leds::set(Pins::LED_MID, Leds::ON, Leds::MEDIUM, 0, 50);

            Commands::publishState();
        }
    }

    namespace Position {

        inline void update() {
            
            uint32_t now = millis();

            switch (_motor.state) {

                case IDLE: break; // If blind is not moving, return

                case WAITING: { // Changing between directions while moving needs a motor safe time

                    Relays::stop();

                    /* Start the timers if it is first waiting period */
                    if (_motor.waitingTime == 0) {_motor.waitingTime = now; break;}

                    /* Check if safe time was enough and start movement */
                    if ((now - _motor.waitingTime) >= (uint32_t) Defaults::MOTOR_SAFE_TIME*10) {
                        _motor.state = MOVING;
                        _motor.startTime = 0;
                        _motor.waitingTime = 0;
                    } break;
                }

                case MOVING: {

                    /* Init all variables and state on first movement */
                    if (_motor.startTime == 0) {
                        _motor.startTime = now;
                        _motor.lastTime = now;
                        _motor.statePublishTime = now;
                        _motor.realPosition = (float) Settings::state.currentPosition;
                        (_motor.direction == UP) ? Relays::up() : Relays::down();
                        break;
                    }

                    /* Publish new state */
                    if (now - _motor.statePublishTime >= 1000) { 
                        _motor.statePublishTime = now;
                        Commands::publishState();
                    }

                    /* Calculate delta time and delta position to modify current position */
                    uint32_t dt = now - _motor.lastTime; _motor.lastTime = now;
                    uint32_t totalTime = (_motor.direction == DOWN) ? Settings::prefs.downTime * 10 : Settings::prefs.upTime * 10;

                    float movedPosition = ((float)dt * 10000.0f) / (float)totalTime;

                    /* Move current position (with excess checks) */
                    if (_motor.direction == UP) (_motor.realPosition < 10000.0f) ? _motor.realPosition += movedPosition : _motor.realPosition = 10000.0f;
                    else if (_motor.direction == DOWN) (_motor.realPosition > movedPosition) ? _motor.realPosition -= movedPosition : _motor.realPosition = 0.0f;

                    /* Update currentPosition on global variable */
                    Settings::state.currentPosition = (uint16_t) _motor.realPosition;


                    /* ~~~ STOP MOVEMENT CRITERIA ~~~ */

                    // Time excess
                    if ((now - _motor.startTime) >= totalTime) stop();

                    // Position excess
                    else if ((_motor.direction == UP && _motor.realPosition >= _motor.nextPosition) || 
                             (_motor.direction == DOWN && _motor.realPosition <= _motor.nextPosition)) {

                        if (_motor.nextPosition == 0 || _motor.nextPosition == 10000) {
                            _motor.state = STOPPING;
                            _motor.startTime = now;

                        } else stop();
                    } break;
                }
                    
                case STOPPING: {
                    if ((now - _motor.startTime) >= STOPPING_TIME) stop(); 
                    break;
                }
            }
        }

        inline void set(uint16_t targetPosition) {

            /* Ignore this function call if the device is at asked position */
            if (targetPosition > Settings::state.currentPosition) {
                if ((targetPosition - Settings::state.currentPosition) < TOL) return;
            } else if ((Settings::state.currentPosition - targetPosition) < TOL) return;

            /* Determine new motor direction and position from current and target position */
            Direction newDirection = (targetPosition > Settings::state.currentPosition) ? UP : DOWN;
            _motor.nextPosition = targetPosition;  // Save new next position on global state

            if (newDirection == _motor.direction) return;
            else _motor.direction = newDirection;

            /* Set the new state for the next update function cycle */
            if (_motor.state == IDLE) {_motor.state = MOVING; _motor.startTime = 0;}
            else {_motor.state = WAITING; _motor.waitingTime = 0;}
        }
    }

    /* Public blinds update */
    inline void update() {
        Position::update();
    }
}

#endif