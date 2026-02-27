#include "pins.h"
#include "settings.h"
#include "defaults.h"

#include <Preferences.h> // Lib to save information on flash memory

Config config;
Prefs prefs;
State state;

Preferences storage;

void reboot() {}

// Clear every division on the storage to set
// default settings again
void factory_reset() {
    storage.begin("storage", false);
    storage.clear();
    storage.end();
    reboot();
}

// Save only the state settings struct, this function will be called just when
// the blind was moving and stops
void save_state() {
    storage.begin("storage", false);
    storage.putBytes("s", &state, sizeof(State));
    storage.end();
}

void save_settings() {

    // Temp variables to store current config and prefs on the flash memory
    Config current_stored_config;
    Prefs current_stored_prefs;

    storage.begin("storage", true); // Open storage on read-only mode (true)

    // Load the more persistent settings divisions from storage to check if they
    // are the same as the intended to save
    storage.getBytes("c", &current_stored_config, sizeof(Config));
    storage.getBytes("p", &current_stored_prefs, sizeof(Prefs));

    storage.end();

    storage.begin("storage", false); // Open storage on write mode (false)

    // Save every settings division to storage if needed and check if every bit
    // of information on current_stored_config and current config are the same.

    if (memcmp(&config, &current_stored_config, sizeof(Config)) != 0)
        storage.putBytes("c", &config, sizeof(Config));

    if (memcmp(&prefs, &current_stored_prefs, sizeof(Prefs)) != 0)
        storage.putBytes("p", &prefs, sizeof(Prefs));

    storage.putBytes("s", &state, sizeof(State));

    storage.end();
}

void load_settings() {

    // Open storage on flash memory on only-read mode (true)
    storage.begin("storage", true);

    // Load every settings division from storage
    size_t r1 = storage.getBytes("c", &config, sizeof(Config));
    size_t r2 = storage.getBytes("p", &prefs, sizeof(Prefs));
    size_t r3 = storage.getBytes("s", &state, sizeof(State));

    storage.end();

    // Check if every block on memory exists and corresponds to its real memory
    // size
    if (r1 != sizeof(Config) || r2 != sizeof(Prefs) || r3 != sizeof(State)) {
        defaults();
        save_settings();
    }
}

void config_setup() { load_settings(); }
