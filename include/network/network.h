#ifndef NETWORK_H
#define NETWORK_H

#include "settings.h"
#include "commands.h"
#include "wifi.h"
#include "mqtt.h"
#include "mode.h"

namespace Network {

    void inline setup() {
        Wifi::setup();
        Mqtt::setup();
        Mqtt::setCallback(Commands::callback);
        Mode::set(Mode::CONNECTION);
    }

    bool inline isConnected() {
        return (Wifi::isConnected() && Mqtt::isConnected());
    }

    void inline update() {
        Wifi::update();
        if (Wifi::isConnected()) Mqtt::update();

        // Leds on normal mode
        if (Mode::is(Mode::Value::NORMAL)) {
            if (Wifi::isOnTimeout() || Mqtt::isOnTimeout()) Leds::set(Pins::LED_GREEN, Leds::ON);
            else Leds::set(Pins::LED_GREEN, Leds::OFF);
        }

        // Leds on connection mode
        if (Mode::is(Mode::Value::CONNECTION)) {
            if (Wifi::isConnected()) Leds::set(Pins::LED_MID, Leds::BLUE);
            else Leds::set(Pins::LED_MID, Leds::RED);

            if (Mqtt::isConnected()) Leds::set(Pins::LED_BTM, Leds::BLUE);
            else Leds::set(Pins::LED_BTM, Leds::RED);
        }
    }

    void inline reconnect() {
        if (!Wifi::isConnected()) Wifi::reconnect();
        else if (!Mqtt::isConnected()) Mqtt::reconnect();
    }
}

#endif // NETWORK_H