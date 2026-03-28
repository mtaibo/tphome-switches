#include <ESP8266WebServer.h>
#include "settings.h"
#include "leds.h"

namespace AP {
    
    ESP8266WebServer server(80);

    const char WEB_PAGE[] PROGMEM = R"rawliteral(
    <!DOCTYPE html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'>
    <style>body{font-family:sans-serif;padding:20px;} input{width:100%;margin-bottom:10px;padding:8px;}</style></head>
    <body><h2>New device config</h2><form action='/save' method='POST'>
    SSID:<input name='s'>Pass:<input name='p' type='password'>
    MQTT IP:<input name='h'>Port:<input name='o' value='1883'>
    User:<input name='u'>Pass:<input name='m' type='password'>
    <button type='submit' style='width:100%;padding:10px'>Guardar</button></form></body></html>)rawliteral";

    void save() {

        strlcpy(Settings::config.wifiSSID, server.arg("s").c_str(), sizeof(Settings::config.wifiSSID));
        strlcpy(Settings::config.wifiPass, server.arg("p").c_str(), sizeof(Settings::config.wifiPass));
        strlcpy(Settings::config.mqttIP,   server.arg("h").c_str(), sizeof(Settings::config.mqttIP));
        strlcpy(Settings::config.mqttUser, server.arg("u").c_str(), sizeof(Settings::config.mqttUser));
        strlcpy(Settings::config.mqttPass, server.arg("m").c_str(), sizeof(Settings::config.mqttPass));
        Settings::config.mqttPort = server.arg("o").toInt();

        Settings::save(); 
        
        server.send(200, "text/plain", "New config saved, rebooting...");
        delay(2000);
        Settings::reboot();
    }

    void start() {
        
        Leds::set(Pins::LED_TOP, Leds::OFF);
        Leds::set(Pins::LED_GREEN, Leds::ON);
        Leds::set(Pins::LED_MID, Leds::OFF);
        Leds::set(Pins::LED_BTM, Leds::OFF);

        WiFi.mode(WIFI_AP);
        WiFi.softAP(Settings::config.deviceID);

        server.on("/", []() {
            server.send_P(200, "text/html", WEB_PAGE);
        });

        server.on("/save", HTTP_POST, save);
        server.begin();
        
        while(true) {
            server.handleClient();
            yield(); 
        }
    }
}