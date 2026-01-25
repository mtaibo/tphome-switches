#include <actions.h>
#include <buttons.h>
#include <config.h>
#include <wifi.h>
#include <pins.h>

int output_pins[] = {LED_TOP, LED_MID, LED_BOTTOM, LED_GREEN, RELAY_UP, RELAY_DOWN};

void setup() {

    // Set configuration to default options
    // TODO: Use config_intit() an previous configuration on the flash memory 
    config_default();

    buttonTop.setup();
    buttonMid.setup();
    buttonBottom.setup();

    // Loop to turn every output pin to output mode
    // TODO: Turn this output pin configuration into an external pin.begin()
    for (int i=0; i<6; i++) {
        pinMode(output_pins[i], OUTPUT);
    }

    // 
    setup_wifi();
    client.setServer(config.mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {

    if (!client.connected()) reconnect();
    client.loop(); 

    buttonTop.check();
    buttonMid.check();
    buttonBottom.check();

    updateActions();
}