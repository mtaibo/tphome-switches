#ifndef NETWORK_H
#define NETWORK_H

#include "settings.h"
#include "commands.h"
#include "wifi.h"
#include "mqtt.h"

namespace Network {

    static bool _wifiWasConnected = false;

    void inline setup() {
        Wifi::setup();
        Mqtt::setup();
   }

    void inline update() {
        Wifi::update();
        if (Wifi::isConnected()) {
            if (!_wifiWasConnected) {
                _wifiWasConnected = true;
                Mqtt::reconnect();
            }
            Mqtt::update();
        } else {
            if (_wifiWasConnected) {
                _wifiWasConnected = false;
                Mqtt::reconnect();
            }
        }
    }
}

#endif // NETWORK_H