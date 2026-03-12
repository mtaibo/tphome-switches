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
        Mqtt::_client.setCallback(Commands::callback);
    }

    void inline update() {
        Wifi::update();
        if (Wifi::isConnected()) {
            if (!_wifiWasConnected) Mqtt::reconnect();
            _wifiWasConnected = true;
            Mqtt::update();
        } else _wifiWasConnected = false;
    }
}

#endif // NETWORK_H