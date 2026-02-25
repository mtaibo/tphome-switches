#ifndef NETWORK_H
#define NETWORK_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

extern WiFiClient espClient;
extern PubSubClient client;

void network_setup();
void network_check();

void access_point();

bool mqtt_reconnect();
void callback(char* topic, byte* payload, unsigned int length);

#endif
