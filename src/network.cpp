#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <actions.h>
#include <config.h>
#include <pins.h>

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {

    // Common messages received on the chip to set main actions
    if (!strcmp(topic, config.set_topic)) { 

        // Orders to go UP, DOWN or to STOP
        if (length == 2 && !memcmp(payload, "UP", 2)) handle_button_action(BTN_TOP, 200); 
        else if(length == 4) {
            if (!memcmp(payload, "STOP", 4)) handle_button_action(BTN_MID, 200);
            else if (!memcmp(payload, "DOWN", 4)) handle_button_action(BTN_BOTTOM, 200);
        }

        // Order to set the position of the blind to a determined position
        if (length >= 4 && !memcmp(payload, "SET", 3)) {
            // TODO: When blind position gets implemented, this will be next
        }
    } 

    // Messages received on the chip from admin to change configuration
    // or to publish configuration elements from the chip
    else if (!strcmp(topic, config.admin_set_topic)) {
        
        // TODO: Possible commands on admin topic are pending to be created

        if (length == 6 && !memcmp(payload, "GET_ID", 6)) {
            client.publish(config.admin_state_topic, config.device_id);
        }

        // 
        else if (length == 10 && !memcmp(payload, "GET_CONFIG", 10)) {
            client.publish(config.admin_state_topic, "DICT");
        } else if (length >= 50 && !memcmp(payload, "SET_CONFIG", 10)) {

        } else if (length == 11 && !memcmp(payload, "SAVE_CONFIG", 11)) {

        } else if (length == 14 && !memcmp(payload, "DEFAULT_CONFIG", 14)) {
            config_default(); save_config();
            client.publish(config.admin_state_topic, "default config restored");
        }

        // Restart the chip to reload its setup and loop
        else if (length == 6 && !memcmp(payload, "REBOOT", 6)) {

        }

        // Position reset messages to prevent the blind to get stuck at any wrong pos
        else if (length == 13 && !memcmp(payload, "RESET_POS_100", 13)) config.current_position = 100;
        else if (length == 12 && !memcmp(payload, "RESET_POS_50", 12)) config.current_position = 50;
        else if (length == 11 && !memcmp(payload, "RESET_POS_0", 11)) config.current_position = 0;
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
    WiFi.begin(config.wifi_ssid, config.wifi_password);
    
    // MQTT server configuration
    client.setServer(config.mqtt_server, config.mqtt_server_port);
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
        
        // Stop led blinking and return true as the connection was succesful
        blink(LED_GREEN, 0); return true;

    } return false;
}

void network_check() {

    // This line activates the green_led while the wifi is not connected
    if (WiFi.status() != WL_CONNECTED) {if (!config.is_blinking) { blink(LED_GREEN, 1); config.blink_time = 10000; } return; } 

    // Gestión de MQTT
    if (!client.connected()) {

        // Section to control blinking led when mqtt server disconnects
        if (!config.is_blinking) blink(LED_GREEN, 1);
        config.blink_time = 500;

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