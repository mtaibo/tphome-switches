#ifndef COMMANDS_H
#define COMMANDS_H

#include "mqtt.h"

namespace Commands {

    enum class Cmd : uint8_t {

        /* Position command goes from 0x00 to 0x64 */

        STOP        = 0xD0,
        OPEN        = 0xD1,
        CLOSE       = 0xD2,
        SET_PREFS   = 0xF0,
    };

    /* Struct to get the new device ID when device ID is no configured */
    struct __attribute__((packed)) DeviceID {
        char    type;   // 'B', 'L'...
        uint8_t zone;   // 01-99
        uint8_t device; // 01-99
    };

    void callback(char* topic, byte* payload, unsigned int length) {

        /* Check if deviced is configured, otherwise listen only for the new deviceID */
        if (strlen(Settings::config.deviceID) == 4) {
            if (strcmp(topic, Mqtt::topics.def) != 0) return;
            if (length != sizeof(DeviceID)) return;

            const auto* id = reinterpret_cast<const DeviceID*>(payload);
            snprintf(Settings::config.deviceID, 6, "%c%02d%02d", id->type, id->zone, id->device);
            Settings::save();
            Settings::reboot();
        }

        /* Empty messages */
        if (length == 0) return;

        if (strcmp(topic, Mqtt::topics.cmd)    == 0 ||
            strcmp(topic, Mqtt::topics.room)   == 0 ||
            strcmp(topic, Mqtt::topics.global) == 0) {

            /* Only one byte needed for this topic */
            if (length != 1) return;

            uint8_t cmd = payload[0]; // Translate first byte of payload to uint8_t

            if (cmd <= 100) Blinds::Position::set(cmd * 100); // cmd*100 to follow uint16_t standard of position

            else switch (static_cast<Cmd>(cmd)) {
                case Cmd::STOP:  Blinds::Relays::stop();                               break;
                case Cmd::OPEN:  Blinds::Position::set(10000);                         break;
                case Cmd::CLOSE: Blinds::Position::set(Settings::prefs.downPosition);  break;
                default: break;
            }
        }

        else if (strcmp(topic, Mqtt::topics.admin) == 0) {
            if (length != sizeof(Settings::Prefs)) return;
            memcpy(&Settings::prefs, payload, sizeof(Settings::Prefs));
            Settings::save();
        }
    }
}

#endif // COMMANDS_H