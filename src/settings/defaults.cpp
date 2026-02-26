#include <string.h>

#include "settings.h"
#include "defaults.h"


void defaults() {

  // ------- WiFi / MQTT Credentials  ---------

  strlcpy(config.wifi_ssid, DEF_WIFI_SSID, sizeof(config.wifi_ssid));
  strlcpy(config.wifi_pass, DEF_WIFI_PASS, sizeof(config.wifi_pass));

  strlcpy(config.mqtt_ip, DEF_MQTT_IP, sizeof(config.mqtt_ip));
  config.mqtt_port = DEF_MQTT_PORT;

  // ---------     Identification     ---------

  strlcpy(config.device_id, DEF_DEVICE_ID, sizeof(config.device_id));
  strlcpy(config.room, DEF_ROOM, sizeof(config.room));
  strlcpy(config.name, DEF_NAME, sizeof(config.name));

  // ---------     Preferences        ---------

  prefs.up_time = DEF_UP_TIME;
  prefs.down_time = DEF_DOWN_TIME;

  prefs.mid_led_time = DEF_STOP_LED_TIME;
  prefs.motor_safe_time = DEF_MOTOR_SAFE_TIME;

  prefs.short_pulse = DEF_SHORT_PULSE;
  prefs.long_pulse = DEF_LONG_PULSE;

  prefs.down_position = DEF_DOWN_POSITION;

  // ---------       States           ---------

  state.current_position = DEF_CURRENT_POSITION;
  state.next_position = DEF_NEXT_POSITION;

  state.active_relay = DEF_ACTIVE_RELAY;
  state.active_led = DEF_ACTIVE_LED;

  state.pending_relay = DEF_PENDING_RELAY;
  state.pending_led = DEF_PENDING_LED;

  state.is_moving = DEF_IS_MOVING;
  state.is_waiting = DEF_IS_WAITING;
  state.is_blinking = DEF_IS_BLINKING;
  state.pause_control = DEF_PAUSE_CONTROL;
}
