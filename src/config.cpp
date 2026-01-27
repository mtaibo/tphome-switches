#include <config.h>
#include <secrets.h>

Config config;

void config_init() {

    config_default();

}


void config_default() {

    // Network configuration
    strcpy(config.wifi_ssid, WIFI_SSID);
    strcpy(config.wifi_password, WIFI_PASS);

    strcpy(config.mqtt_server, MQTT_SERVER);
    strcpy(config.mqtt_user, MQTT_USER);
    strcpy(config.mqtt_pass, MQTT_PASS);

    // Blind configuration
    config.up_time = 30000;
    config.down_time = 25000;
    config.current_position = 0.0;

    // Execution data
    config.is_moving = false;
    config.is_waiting = false;

    // Preferences
    config.block_buttons = false;
    config.stop_led_time = 750;
    config.short_pulse = 3000;
    config.long_pulse = 10000;
}