#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <config.h>
#include <actions.h>
#include <pins.h>

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {

    String message = "";

    // Loop that gets the callback message from its bytes and length
    for (unsigned int i = 0; i < length; i++) message += (char)payload[i];

    // 
    if (String(topic) == "tphome/blinds/") {
        if (message == "UP")    handleButtonAction(BTN_TOP, 200); 
        else if (message == "STOP") handleButtonAction(BTN_MID, 200);
        else if (message == "DOWN") handleButtonAction(BTN_BOTTOM, 200);
    }
}

void network_setup() {

    // Wi-Fi configuration
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.wifi_ssid, config.wifi_password);
    
    // MQTT server configuration
    client.setServer(config.mqtt_server, 1883);
    client.setCallback(callback);
}

// Function to connect to the MQTT server with its credentials
// and also to subscribe to the corresponding topic
bool mqtt_reconnect() {
    if (client.connect("Persiana_ESP8266", config.mqtt_user, config.mqtt_pass)) {
        client.subscribe("casa/persiana/comando"); return true;}
    return false;
}

void network_check() {

    // This line activates the green_led while the wifi is not connected
    if (WiFi.status() != WL_CONNECTED) { if (millis() - config.last_blink >= 500) blink(LED_GREEN, 1); return; } 
    
    // This line deactivates the blinking when the wifi is connected
    if (config.is_blinking) blink(LED_GREEN, 0);

    // Gestión de MQTT
    if (!client.connected()) {

        // This lines below manage a 5 second delay 
        // to reattempt the connection to the MQTT server
        static unsigned long last_reconnect_attempt = 0;
        if (millis() - last_reconnect_attempt > 5000) { 
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