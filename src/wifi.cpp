#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <config.h>
#include <actions.h>
#include <pins.h>

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastReconnectAttempt = 0;

void callback(char* topic, byte* payload, unsigned int length) {

    String message = "";

    // 
    for (unsigned int i = 0; i < length; i++) message += (char)payload[i];

    if (String(topic) == "casa/persiana/comando") {
        if (message == "UP")    handleButtonAction(BTN_TOP, 200); 
        else if (message == "STOP") handleButtonAction(BTN_MID, 200);
        else if (message == "DOWN") handleButtonAction(BTN_BOTTOM, 200);
    }
}

void setup_wifi() {
    delay(10);
    WiFi.begin(config.wifi_ssid, config.wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void setup_mqtt() {
    client.setServer(config.mqtt_server, 1883);
    client.setCallback(callback);
}

bool reconnect() {
    if (client.connect("Persiana_ESP8266", config.mqtt_user, config.mqtt_pass)) {
        client.subscribe("casa/persiana/comando");
        return true;
    }
    return false;
}

void check_connections() {

    if (WiFi.status() != WL_CONNECTED) return; 

    if (!client.connected()) {
        unsigned long now = millis();
        if (now - lastReconnectAttempt > 5000) { 
            lastReconnectAttempt = now;
            if (reconnect()) {
                lastReconnectAttempt = 0;
            }
        }
    } else {
        client.loop();
    }
}