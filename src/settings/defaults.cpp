#include <string.h>

#include "settings.h"
#include "defaults.h"

void defaults() {

    auto& c = Settings::config;
    auto& p = Settings::prefs;
    auto& s = Settings::state;

    // ---------     Identification     ---------

    strlcpy(c.device_id, DEF_DEVICE_ID, sizeof(c.device_id));
    strlcpy(c.room, DEF_ROOM, sizeof(c.room));
    strlcpy(c.name, DEF_NAME, sizeof(c.name));

    // ------- WiFi / MQTT Credentials  ---------

    strlcpy(c.wifi_ssid, DEF_WIFI_SSID, sizeof(c.wifi_ssid));
    strlcpy(c.wifi_pass, DEF_WIFI_PASS, sizeof(c.wifi_pass));

    strlcpy(c.mqtt_ip, DEF_MQTT_IP, sizeof(c.mqtt_ip));
    strlcpy(c.mqtt_user, DEF_MQTT_USER, sizeof(c.mqtt_user));
    strlcpy(c.mqtt_pass, DEF_MQTT_PASS, sizeof(c.mqtt_pass));
    c.mqtt_port = DEF_MQTT_PORT;

    /* BLIND DEFAULT SETTINGS */

#if defined(DEVICE_TYPE_BLIND)

    // ---------     Preferences        ---------

    p.up_time   = DEF_UP_TIME;
    p.down_time = DEF_DOWN_TIME;

    p.mid_led_time    = DEF_STOP_LED_TIME;
    p.motor_safe_time = DEF_MOTOR_SAFE_TIME;

    p.short_pulse = DEF_SHORT_PULSE;
    p.long_pulse  = DEF_LONG_PULSE;

    p.down_position = DEF_DOWN_POSITION;

    // ---------       States           ---------

    s.current_position = DEF_CURRENT_POSITION;
    s.next_position    = DEF_NEXT_POSITION;

    s.active_relay = DEF_ACTIVE_RELAY;

    s.pending_relay = DEF_PENDING_RELAY;

    s.is_moving     = DEF_IS_MOVING;
    s.is_waiting    = DEF_IS_WAITING;

    /* LIGHT DEFAULT SETTINGS */

#elif defined(DEVICE_TYPE_LIGHT)

#endif
}
