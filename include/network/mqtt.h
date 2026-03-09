#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include "settings.h"
#include "wifi.h"

namespace Mqtt {

    static char t_cmd[24];   
    static char t_room[24];  
    static char t_state[24]; 

    WiFiClient _wifiClient;
    PubSubClient _client(_wifiClient);

    void callback(char* topic, byte* payload, unsigned int length) {
        if (length == 0) return;

        if (length == 1) {
            uint8_t cmd = payload[0];
            if (cmd <= 100) {
                // Position Logic
            } else {
                switch (cmd) {
                    case 0xD0: /* STOP */ break;
                    case 0xD1: /* UP */   break;
                    case 0xD2: /* DOWN */ break;
                }
            }
        } 
        
        /* Prefs update */
        else if (length == sizeof(Settings::Prefs)) {
            memcpy(&Settings::prefs, payload, sizeof(Settings::Prefs));
            Settings::save(); // Guardado directo
        }
    }

    void setup() {
        
        /* Create topics */
        snprintf(t_cmd, sizeof(t_cmd), "d/%s/c", Settings::config.deviceID);
        snprintf(t_room, sizeof(t_room), "g/%s/c", Settings::config.room);
        snprintf(t_state, sizeof(t_state), "d/%s/s", Settings::config.deviceID);

        _client.setServer(Settings::config.mqttIP, Settings::config.mqttPort);
        
        /* Assign max buffer size and 64 more bits to prevent collapse */
        _client.setBufferSize(sizeof(Settings::Prefs) + 64); 
        
        _client.setCallback(callback);
    }

    void update() {

        /* Only update mqtt server if wifi is connected */
        if (!Wifi::_state.isConnected) return;

        if (!_client.connected()) {

            if (_client.connect(Settings::config.deviceID, Settings::config.mqttUser, Settings::config.mqttPass)) {
                _client.subscribe(t_cmd);
                _client.subscribe(t_room);
            }
        } else {
            _client.loop();
        }
    }

    void sendState() {
        if (!_client.connected()) return;
        _client.publish(t_state, (uint8_t*)&Settings::state, sizeof(Settings::State), true);
    }
}

#endif // MQTT_H