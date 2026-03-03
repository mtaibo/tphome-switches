#include "network.h"
#include "buttons.h"
#include "actions.h"
#include "settings.h"

#if defined(DEVICE_HARDWARE_ESP8266)
    #include "hardware/esp8266/hardware.h"
#elif defined(DEVICE_HARDWARE_BK7231N)
    #include "hardware/bk7231n/hardware.h"
#endif

void setup() {

    Settings::setup();
    Hardware::setup();

    network_setup();
}

void loop() {

    update_actions();
    network_check();

    buttonTop.check();
    buttonMid.check();
    buttonBottom.check();

    delay(10);
}
