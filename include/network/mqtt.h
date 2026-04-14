#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>

#include "settings.h"
#include "wifi.h"

#define ATTEMPT_INTERVAL 5000
#define MAX_ATTEMPTS 5

namespace Mqtt {

    struct State {
        uint32_t lastTime = 0;
        uint8_t attempts = 0;
        bool isConnected = false;
        bool ledOn = false;
    };

    struct Topics {

        char def[Sizes::MQTT]; // Default non-configured device topic
        char global[Sizes::MQTT]; // Subscribe to global orders on same device type

        /* Device topics */
        char cmd[Sizes::MQTT];   // Subscribe to common orders
        char admin[Sizes::MQTT]; // Subscribe to administrate device
        char state[Sizes::MQTT]; // Publish device state
    };

    static Topics topics;

    static State _state;
    static WiFiClient _wifiClient;
    static PubSubClient _client(_wifiClient);

    /* Function to set callback on network.h module and prevent circular include
     * when including mqtt.h module on commands.h module. */
    void setCallback(MQTT_CALLBACK_SIGNATURE) {
        _client.setCallback(callback);
    }

    bool isConnected() {
        return _state.isConnected;
    }
        
    inline bool isOnTimeout() {
        return (!_state.isConnected) && (_state.attempts >= MAX_ATTEMPTS);
    }

    void setup() {

        /* When the device is not configured, its device ID has 4 hex digits
         * and the device has to subscribe to the def topic to get its new ID */
        if (strlen(Settings::config.deviceID) == 4) {
            snprintf(topics.def, Sizes::MQTT, "def/%s/a", Settings::config.deviceID);
            snprintf(topics.state, Sizes::MQTT, "def/%s/s",  Settings::config.deviceID);
        }

        else { // Topics for configured devices
            snprintf(topics.cmd,   Sizes::MQTT, "tp/%s/c",  Settings::config.deviceID);
            snprintf(topics.admin, Sizes::MQTT, "tp/%s/a",  Settings::config.deviceID);
            snprintf(topics.state, Sizes::MQTT, "tp/%s/s",  Settings::config.deviceID);
        }
            
        // Build global topic
        snprintf(topics.global, Sizes::MQTT, "tp/a/c");

        /* Setup the mqtt server with its credentials, max buffer and callback function */
        _client.setServer(Settings::config.mqttIP, Settings::config.mqttPort);
        _client.setBufferSize(Sizes::MQTT_BUFFER); 
        _client.setSocketTimeout(10); 
    }

    void reconnect() {

        /* Restart default values on connection state */
        _state.ledOn = false;
        _state.lastTime = 0;
        _state.attempts = 0;
        _state.isConnected = false;

        /* Start reconnection by setting up again */
        setup();
    }

    void update() {

        uint32_t now = millis();

        /* While device is connected */
        if (_state.isConnected) {

            /* Disconnected after being connected, do not reconnect until user ask for it */
            if (!_client.connected()) {
                _state.isConnected = false;
                _state.lastTime = now;
            } 

            else _client.loop();
            return;
        }

        /* While device is disconnected */
        if (!_client.connected()) {

            /* First connection attempt: wait 3s to stabilize wifi connection */
            if (_state.lastTime == 0) {_state.lastTime = now + 3000; return;}

            if (_state.attempts < MAX_ATTEMPTS) { // Not more than 5 attempts per manual reconnection

                /* Check interval between attempts */
                if (now - _state.lastTime > ATTEMPT_INTERVAL) {

                    /* First attempt, turn led on */
                    if (_state.ledOn == false) {
                        Leds::set(Pins::LED_GREEN, Leds::BLINK, Leds::SLOW);
                        _state.ledOn = true;
                    }

                    const char offlineByte = 0xFF; // LWT byte

                    _client.connect(
                        Settings::config.deviceID, 
                        Settings::config.mqttUser, 
                        Settings::config.mqttPass,

                        /* LWT lines */
                        topics.state, 1, false,
                        &offlineByte, 1
                    );

                    _state.lastTime = now;
                    _state.attempts++;
                }

            } else if (_state.ledOn) {Leds::set(Pins::LED_GREEN, Leds::OFF); _state.ledOn = false;}
        } 
        
        else { // Client is connected but state.isConnected is false === first connection
            if (_state.ledOn) {Leds::set(Pins::LED_GREEN, Leds::OFF); _state.ledOn = false;}
            _state.isConnected = true;
            _state.attempts = MAX_ATTEMPTS;
            _state.lastTime = now;

            if (strlen(Settings::config.deviceID) == 4) _client.subscribe(topics.def);
            else {
                _client.subscribe(topics.cmd);
                _client.subscribe(topics.admin);
            }

            _client.subscribe(topics.global);
        }
    }
}

#endif // MQTT_H