#ifndef WIFI
#define WIFI

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Different objects from the imports above
// to control the Wifi of the chip and the MQTT server
extern WiFiClient espClient;
extern PubSubClient client;

void network_setup();
void network_check();

void mqtt_reconnect();
void callback(char* topic, byte* payload, unsigned int length);

#endif