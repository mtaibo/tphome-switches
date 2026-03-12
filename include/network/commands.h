#ifndef COMMANDS_H
#define COMMANDS_H

#include "blinds.h"
#include "mqtt.h"

namespace Commands {

    #if defined(DEVICE_TYPE_BLIND)

        enum class Cmd : uint8_t {
            UP   = 0xC0,
            DOWN = 0xC1,
            STOP = 0xC2,
        };

        enum class State : uint8_t {
            IDLE        = 0x00,
            MOVING_UP   = 0x01,
            MOVING_DOWN = 0x02,
            ERROR       = 0x03,
        };

    #elif defined(DEVICE_TYPE_LIGHT)
    #endif

    /* Assign a new deviceID to an unconfigured device */
    struct __attribute__((packed)) DeviceID {
        char    type;   // 'B', 'L', ...
        uint8_t zone;   // 01–99
        uint8_t device; // 01–99
    };

    /* Device state report */
    struct __attribute__((packed)) StatePayload {
        uint8_t position;
        State   state;
    };

    /* Publish state */
    void publishState(uint8_t position, State state) {
        StatePayload payload { position, state };
        Mqtt::_client.publish(Mqtt::topics.state,
                             reinterpret_cast<const uint8_t*>(&payload),
                             sizeof(StatePayload));
    }

    static void handleCmd(uint8_t cmd) {

        #if defined(DEVICE_TYPE_BLIND)

            if (cmd <= 100) {
                Blinds::Position::set(cmd * 100);
                return;
            }
            switch (static_cast<Cmd>(cmd)) {
                case Cmd::UP:   Blinds::Position::set(10000);                        break;
                case Cmd::DOWN: Blinds::Position::set(Settings::prefs.downPosition); break;
                case Cmd::STOP: Blinds::Relays::stop();                              break;
                default: break;
            }

        #elif defined(DEVICE_TYPE_LIGHT)
        #endif
    }

    void callback(char* topic, byte* payload, unsigned int length) {

        /* When deviceID is in its default value, wait until 
         * be configured to execute other commands */
        if (strlen(Settings::config.deviceID) == 4) {
            if (strcmp(topic, Mqtt::topics.def) != 0) return;
            if (length != sizeof(DeviceID))            return;

            const auto* id = reinterpret_cast<const DeviceID*>(payload);
            snprintf(Settings::config.deviceID, 6, "%c%02d%02d",
                     id->type, id->zone, id->device);
            Settings::save();
            Settings::reboot();
            return;
        }

        if (length == 0) return;

        /* Common commands on cmd, room and global topics */
        if (strcmp(topic, Mqtt::topics.cmd)    == 0 ||
            strcmp(topic, Mqtt::topics.room)   == 0 ||
            strcmp(topic, Mqtt::topics.global) == 0) {

            if (length != 1) return;
            handleCmd(payload[0]);
        }

        /* Admin commands on admin topic to change Prefs variables */
        else if (strcmp(topic, Mqtt::topics.admin) == 0) {
            if (length != sizeof(Settings::Prefs)) return;
            memcpy(&Settings::prefs, payload, sizeof(Settings::Prefs));
            Settings::save();
        }
    }

}

#endif // COMMANDS_H