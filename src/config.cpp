#include <Preferences.h>

#include "default.h"
#include "config.h"
#include "pins.h"

Config config;
Preferences prefs;

void reboot() {

    // Make the main leds blink to make user aware of a reboot
    for (unsigned int i = 0; i < 3; i++) {
      digitalWrite(LED_TOP, HIGH);
      digitalWrite(LED_MID, HIGH);
      digitalWrite(LED_BOTTOM, HIGH);
      delay(1000);
      digitalWrite(LED_TOP, LOW);
      digitalWrite(LED_MID, LOW);
      digitalWrite(LED_BOTTOM, LOW);
      delay(1000);
    }

    ESP.restart();
}

void load_config() {

    // * Set configuration

    // Execution data
    config.is_moving = false;
    config.is_waiting = false;
    config.network_timeout = false;

    config.active_relay = -1;
    config.active_led = -1;
    config.pending_relay = -1;
    config.pending_led = -1;

    // Time execution data
    config.pause_control = false;
    config.stop_time = 0;
    config.stop_led_time = 0;
    config.current_time_limit = 0;
    config.last_cycle_time = 0;

    // Blinking config
    config.is_blinking = false;
    config.blinking_led = -1;
    config.blinking_state = -1;
    config.blink_time = 500;
    config.last_blink = 0;

    // Preferences
    config.mid_led_time = 500;
    config.motor_safe_time = 1000;

    config.short_pulse = 3000;
    config.long_pulse = 10000;


    // * Load config from flash memory, if not found, load default settings

    prefs.begin("prefs", true);

    // Device ID
    if (!prefs.isKey("id")) strcpy(config.device_id, DEVICE_ID);
    else prefs.getString("id", config.device_id, sizeof(config.device_id));

    // WIFI
    if (!prefs.isKey("ssid")) strcpy(config.wifi_ssid, WIFI_SSID);
    else prefs.getString("ssid", config.wifi_ssid, sizeof(config.wifi_ssid));
    if (!prefs.isKey("wifi_pass")) strcpy(config.wifi_pass, WIFI_PASS);
    else prefs.getString("wifi_pass", config.wifi_pass, sizeof(config.wifi_pass));

    // MQTT
    if (!prefs.isKey("ip")) strcpy(config.mqtt_server, MQTT_SERVER);
    else prefs.getString("ip", config.mqtt_server, sizeof(config.mqtt_server));

    config.mqtt_port = prefs.getUInt("port", MQTT_PORT);
    
    if (!prefs.isKey("user")) strcpy(config.mqtt_user, MQTT_USER);
    else prefs.getString("user", config.mqtt_user, sizeof(config.mqtt_user));
    if (!prefs.isKey("mqtt_pass")) strcpy(config.mqtt_pass, MQTT_PASS);
    else prefs.getString("mqtt_pass", config.mqtt_pass, sizeof(config.mqtt_pass));

    // Topic
    if (!prefs.isKey("type")) strcpy(config.type, TYPE);
    else prefs.getString("type", config.type, sizeof(config.type));
    if (!prefs.isKey("room")) strcpy(config.room, ROOM);
    else prefs.getString("room", config.room, sizeof(config.room));
    if (!prefs.isKey("name")) strcpy(config.name, NAME);
    else prefs.getString("name", config.name, sizeof(config.name));

    // Blind config
    config.up_time = prefs.getULong("up_time", UP_TIME);
    config.down_time = prefs.getULong("down_time", DOWN_TIME);
    config.current_position = prefs.getFloat("pos", 100.0);
    config.down_position = prefs.getFloat("down_pos", 20.0);

    config.next_position = config.current_position;

    prefs.end();
}

void save_config() {

    prefs.begin("prefs", false);

    // Device ID
    prefs.putString("id", config.device_id);

    // WIFI 
    prefs.putString("ssid", config.wifi_ssid);
    prefs.putString("wifi_pass", config.wifi_pass);

    // MQTT 
    prefs.putString("ip", config.mqtt_server);
    prefs.putUInt("port", config.mqtt_port);
    prefs.putString("user", config.mqtt_user);
    prefs.putString("mqtt_pass", config.mqtt_pass);

    // Topic 
    prefs.putString("type", config.type);
    prefs.putString("room", config.room);
    prefs.putString("name", config.name);

    // Blind config
    prefs.putULong("up_time", config.up_time);
    prefs.putULong("down_time", config.down_time);
    prefs.putFloat("pos", config.current_position);
    prefs.putFloat("down_pos", config.down_position);

    prefs.end();
    reboot();
}

void reset_memory() {
    prefs.begin("prefs", false);
    prefs.clear();
    prefs.end();
    delay(2000);
    reboot();
}

void pin_setup() {

   // This both lines below activate the pin to allow
    // the blue leds to turn on, otherwise, they'll be blocked
    pinMode(CONFIG_LED, OUTPUT);
    digitalWrite(CONFIG_LED, HIGH);

    // Loop to turn every output pin to output mode and low
    int output_pins[] = {LED_TOP, LED_MID, LED_BOTTOM, LED_GREEN, RELAY_UP, RELAY_DOWN};
    for (int i=0; i<6; i++) {
        pinMode(output_pins[i], OUTPUT);
        digitalWrite(output_pins[i], LOW);
    }
}

void config_setup() {
    pin_setup();
    load_config();
}
