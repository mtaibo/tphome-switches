#include "settings.h"
#include "defaults.h"
#include "leds.h"

#include <Preferences.h> // Lib to save information on flash memory

namespace Settings {
    
    Config config;
    Prefs prefs;
    State state;

    Preferences storage;

    void reboot() {
        Leds::set(LED_TOP, Leds::BLINK, Leds::SLOW, 3);
        Leds::set(LED_MID, Leds::BLINK, Leds::SLOW, 3);
        Leds::set(LED_BTM, Leds::BLINK, Leds::SLOW, 3);
        ESP.restart();
    }

    // Clear every division on the storage to set
    // default settings again
    void reset() {
        storage.begin("storage", false);
        storage.clear();
        storage.end();
        reboot();
    }

    void defaults() {

        /* --------------   Identification  -------------- */
        strlcpy(config.device_id, Defaults::ID, IDENTITY_SIZE);
        strlcpy(config.room, Defaults::ROOM, IDENTITY_SIZE);
        strlcpy(config.name, Defaults::NAME, IDENTITY_SIZE);

        /* -----------------   Network   ----------------- */
        strlcpy(config.wifi_ssid, WIFI_SSID, WIFI_SIZE);
        strlcpy(config.wifi_pass, WIFI_PASS, WIFI_SIZE);
        strlcpy(config.mqtt_ip, MQTT_IP, MQTT_SIZE);
        strlcpy(config.mqtt_user, MQTT_USER, MQTT_SIZE);
        strlcpy(config.mqtt_pass, MQTT_PASS, MQTT_SIZE);
        config.mqtt_port = MQTT_PORT;

        /* --------  Preferences & Initial State  -------- */
        #if defined(DEVICE_TYPE_BLIND)
            prefs.up_time = Defaults::UP_TIME;
            prefs.down_time = Defaults::DOWN_TIME;
            prefs.down_position = Defaults::DOWN_POSITION;
            prefs.inverted_relays = Defaults::INVERTED_RELAYS;
            state.current_position = Defaults::START_POSITION;
            state.current_position = Defaults::START_POSITION;
            state.is_moving = Defaults::IS_MOVING;
        #endif
        
        save();
    }

    // Save only the state settings struct, this function will be called just when
    // the blind was moving and stops
    void saveState() {
        storage.begin("storage", false);
        storage.putBytes("s", &state, sizeof(State));
        storage.end();
    }

    void save() {

        // Temp variables to store current config and prefs on the flash memory
        Config current_stored_config;
        Prefs current_stored_prefs;

        storage.begin("storage", false); // Open storage on write mode (false)

        // Load the more persistent settings divisions from storage to check if they
        // are the same as the intended to save
        storage.getBytes("c", &current_stored_config, sizeof(Config));
        storage.getBytes("p", &current_stored_prefs, sizeof(Prefs));

        // Save every settings division to storage if needed and check if every bit
        // of information on current_stored_config and current config are the same.

        if (memcmp(&config, &current_stored_config, sizeof(Config)) != 0)
            storage.putBytes("c", &config, sizeof(Config));

        if (memcmp(&prefs, &current_stored_prefs, sizeof(Prefs)) != 0)
            storage.putBytes("p", &prefs, sizeof(Prefs));

        storage.putBytes("s", &state, sizeof(State));

        storage.end();
    }

    void load() {

        // Open storage on flash memory on only-read mode (true)
        storage.begin("storage", true);

        // Load every settings division from storage
        size_t r1 = storage.getBytes("c", &config, sizeof(Config));
        size_t r2 = storage.getBytes("p", &prefs, sizeof(Prefs));
        size_t r3 = storage.getBytes("s", &state, sizeof(State));

        storage.end();

        // Check if every block on memory exists and corresponds to its real memory size
        if (r1 != sizeof(Config) || r2 != sizeof(Prefs) || r3 != sizeof(State)) defaults();
    }

    void setup() {defaults();}
}