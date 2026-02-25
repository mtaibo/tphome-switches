#include "network.h"
#include "actions.h"
#include "config.h"
#include "pins.h"

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {

    // Messages received on the set_topic to habndle basic
    // actions, like going up, down, stop and set a position
    if (!strcmp(topic, config.set_topic)) { 

        // Orders to go UP, DOWN or to STOP
        if (length == 2 && !memcmp(payload, "UP", 2)) handle_button_action(BTN_TOP, 200); 
        else if(length == 4) {
            if (!memcmp(payload, "STOP", 4)) handle_button_action(BTN_MID, 200);
            else if (!memcmp(payload, "DOWN", 4)) handle_button_action(BTN_BOTTOM, 200);
        }

        // Order to set the position of the blind to a determined position
        if (length >= 5 && !memcmp(payload, "SET:", 4)) {

          // Create a temp buffer to manage the numbers introduced
          // on the payload received on the callback and introduce
          // a '\0' char to prevent unreadable chars from payload via atof();
          int buffer_length = length - 4;
          char buffer[buffer_length+1];
          buffer[buffer_length] = '\0';

          // Introduce payload content from pos 4 on the buffer
          // where the '\0' was just introduced at the last position
          memcpy(buffer, &payload[4], buffer_length);
          set_position(atof(buffer));
        }

        // Order to get the current position of the blind
        if (length == 7 && !memcmp(payload, "GET_POS", 7)) {
          char buffer[10];
          dtostrf(config.current_position, 1, 2, buffer);
          client.publish(config.state_topic, buffer);
        }
    }

    // Messages received on the chip from admin to change configuration
    // or to publish configuration elements from the chip
    else if (!strcmp(topic, config.admin_set_topic)) {

        // Command to get blind id but useful as a ping/pong command
        if (length == 6 && !memcmp(payload, "GET_ID", 6)) {
            client.publish(config.admin_state_topic, config.device_id);
        }

        // Commands to get the current topics 
        if (length == 13 && !memcmp(payload, "GET_SET_TOPIC", 13)) {
          client.publish(config.admin_state_topic, config.set_topic);}
        if (length == 15 && !memcmp(payload, "GET_STATE_TOPIC", 15)) {
          client.publish(config.admin_state_topic, config.state_topic);}
        if (length == 19 && !memcmp(payload, "GET_ADMIN_SET_TOPIC", 19)) {
          client.publish(config.admin_state_topic, config.admin_set_topic);}
        if (length == 21 && !memcmp(payload, "GET_ADMIN_STATE_TOPIC", 21)) {
          client.publish(config.admin_state_topic, config.admin_state_topic);}

       // Settings configuration commands
        else if (length >= 10 && !memcmp(payload, "GET_CONFIG", 10)) {

            String msg = "";
            String response = "UNKNOWN_KEY";
            for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];

            if (msg.indexOf(':') != -1) {
                String key = msg.substring(msg.indexOf(':') + 1); 
                if (key == "device_id") response = config.device_id;
                else if (key == "wifi_ssid") response = config.wifi_ssid;
                else if (key == "wifi_pass") response = config.wifi_pass;
                else if (key == "mqtt_server") response = config.mqtt_server;
                else if (key == "mqtt_port") response = config.mqtt_port;
                else if (key == "mqtt_user") response = config.mqtt_user;
                else if (key == "mqtt_pass") response = config.mqtt_pass;
                else if (key == "type") response = config.type;
                else if (key == "room") response = config.room;
                else if (key == "name") response = config.name;
                else if (key == "up_time") response = String(config.up_time);
                else if (key == "down_time") response = String(config.down_time);
                else if (key == "current_position") response = String(config.current_position);
                else if (key == "down_position") response = String(config.down_position);
            } else response = "ERROR: Format is GET_CONFIG:key";

            client.publish(config.admin_state_topic, response.c_str());

        } else if (length >= 10 && !memcmp(payload, "SET_CONFIG", 10)) {
            String msg = "";
            for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];

            int firstColon = msg.indexOf(':');
            int secondColon = msg.indexOf(':', firstColon + 1);

            if (firstColon != -1 && secondColon != -1) {
                String key = msg.substring(firstColon + 1, secondColon);
                String value = msg.substring(secondColon + 1);

                if (key == "device_id") strncpy(config.device_id, value.c_str(), sizeof(config.device_id));
                else if (key == "wifi_ssid") strncpy(config.wifi_ssid, value.c_str(), sizeof(config.wifi_ssid));
                else if (key == "wifi_pass") strncpy(config.wifi_pass, value.c_str(), sizeof(config.wifi_pass));
                else if (key == "mqtt_server") strncpy(config.mqtt_server, value.c_str(), sizeof(config.mqtt_server)); 
                else if (key == "mqtt_port") config.mqtt_port = value.toInt();
                else if (key == "mqtt_user") strncpy(config.mqtt_user, value.c_str(), sizeof(config.mqtt_user));
                else if (key == "mqtt_pass") strncpy(config.mqtt_pass, value.c_str(), sizeof(config.mqtt_pass)); 
                else if (key == "type") strncpy(config.type, value.c_str(), sizeof(config.type)); 
                else if (key == "room") strncpy(config.room, value.c_str(), sizeof(config.room));
                else if (key == "name") strncpy(config.name, value.c_str(), sizeof(config.name)); 
                else if (key == "up_time") config.up_time = value.toInt();
                else if (key == "down_time") config.down_time = value.toInt();
                else if (key == "current_position") config.current_position = value.toFloat();
                else if (key == "down_position") config.down_position = value.toFloat();
                
                client.publish(config.admin_state_topic, ("CONF_SAVED: " + key).c_str());
            }

        } else if (length == 11 && !memcmp(payload, "SAVE_CONFIG", 11)) save_config();

        else if (length == 12 && !memcmp(payload, "NEW_FIRMWARE", 12)) {

          // Intializing communication
          ArduinoOTA.setHostname(config.device_id);
          ArduinoOTA.begin();

          client.publish(config.admin_state_topic, String("Prepared to receive firmware on " + WiFi.localIP().toString() + "at 8266").c_str());
          delay(500);

          // Disconnecting MQTT server to give power only for ArduinoOTA
          client.disconnect();
          digitalWrite(CONFIG_LED, LOW); // Signal that mqtt was disconnected

          while (true) {
            ArduinoOTA.handle();
            yield();
          } reboot();
        }

        // Restart the chip to reload its setup with previous configs and loop
        else if (length == 6 && !memcmp(payload, "REBOOT", 6)) reboot();

        // Restart the chip to reload its default values, setup and loop
        else if (length == 12 && !memcmp(payload, "RESET_MEMORY", 12)) {
            client.publish(config.admin_state_topic, " restoring default config"); reset_memory();}
    }
}

void topic_setup() {

    // This lines create the topics depending on the device room, name and device_id
    snprintf(config.set_topic, sizeof(config.set_topic), "tphome/blinds/%s/%s/set", config.room, config.name);
    snprintf(config.state_topic, sizeof(config.state_topic), "tphome/blinds/%s/%s/state", config.room, config.name);
    snprintf(config.admin_set_topic, sizeof(config.admin_set_topic), "tphome/admin/%s/set", config.device_id);
    snprintf(config.admin_state_topic, sizeof(config.admin_state_topic), "tphome/admin/%s/state", config.device_id);

    client.disconnect();
}

void network_setup() {

    // Wi-Fi configuration
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.wifi_ssid, config.wifi_pass);

    // MQTT server configuration
    client.setServer(config.mqtt_server, config.mqtt_port);
    client.setCallback(callback);

    // Topic setup
    topic_setup();
}

bool mqtt_reconnect() {

    // Connect to the MQTT server with its credentials
    if (client.connect(config.device_id, config.mqtt_user, config.mqtt_pass)) {
        
        // When connected, this two lines below subscribe the client 
        // to the set topics to receive changes from the broker
        client.subscribe(config.set_topic); 
        client.subscribe(config.admin_set_topic);

        // Message sent when connection was succesful
        client.publish(config.state_topic, "CONNECTED", true);
        client.publish(config.admin_state_topic, "CONNECTED", true);
        
        // Stop led blinking and return true as the connection was succesful
        blink(LED_GREEN, 0); return true;

    } return false;
}

void network_check() {

    // This line activates the green_led while the wifi is not connected
    if (WiFi.status() != WL_CONNECTED) {if (!config.is_blinking) blink(LED_GREEN, 1); return; } 

    // Gestión de MQTT
    if (!client.connected()) {

        // Section to control blinking led when mqtt server disconnects
        if (!config.is_blinking) blink(LED_GREEN, 1);

        // This lines below manage a 4 second delay 
        // to reattempt the connection to the MQTT server
        static unsigned long last_reconnect_attempt = 0;
        if (millis() - last_reconnect_attempt > 4000) { 
            last_reconnect_attempt = millis();

            // This line calls the mqtt_reconnect function 
            // to check if the connection is available and successful
            if (mqtt_reconnect()) last_reconnect_attempt = 0;
        }
    }

    // This line continues the loop of the client while Wi-Fi and MQTT server are connected,
    // the client loop is where topics and messages are redirected to callback 
    else client.loop();
}
