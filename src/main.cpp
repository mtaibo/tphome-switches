#include <Arduino.h>

#include "config.h"
#include "pins.h"

int input_pins[] = {BTN_TOP, BTN_BOTTOM};
int output_pins[] = {LED_TOP, LED_MID, LED_BOTTOM, LED_GREEN, RELAY_UP, RELAY_DOWN};

int button_pressed = -1;
unsigned long time_pressed = 0;

void setup() {
    Serial.begin(115200);
    config_init();

    // Loop to turn every output pin to output mode
    for (int i=0; i<6; i++) {
        pinMode(output_pins[i], OUTPUT);
    }
    // Loop to turn every input pin to input_pullup mode
    for (int i=0; i<2; i++) {
        pinMode(input_pins[i], INPUT_PULLUP);
    }
}

void loop() {

    if (digitalRead(BTN_TOP) == LOW && button_pressed == -1) {

        button_pressed = BTN_TOP;
        time_pressed = millis();
        unsigned long duration = 0;

        while (digitalRead(BTN_TOP) == LOW) {
            duration = millis() - time_pressed;
            delay(10); 
        }

        Serial.print("Boton soltado. Duracion: ");
        Serial.println(duration);

        Serial.print("Config - Corto: "); Serial.print(config.short_pulse);
        Serial.print(" Largo: "); Serial.println(config.long_pulse);
        Serial.print("Duracion detectada: "); Serial.println(duration);


        if (duration < config.short_pulse) {
            Serial.println("ACCION CORTA");
            digitalWrite(LED_TOP, HIGH);
            digitalWrite(RELAY_UP, HIGH);
            delay(3000);
            digitalWrite(LED_TOP, LOW);
            digitalWrite(RELAY_UP, LOW);
        } else if (duration < config.long_pulse) {
            Serial.println("ACCION MEDIA");
            for (int i=0; i<10; i++) {
                digitalWrite(LED_TOP, HIGH); delay(200);
                digitalWrite(LED_TOP, LOW);  delay(200);
            }
        } else {
            Serial.println("ACCION LARGA");
            for (int i=0; i<10; i++) {
                digitalWrite(LED_GREEN, HIGH); delay(200);
                digitalWrite(LED_GREEN, LOW);  delay(200);
            }
        }

        button_pressed = -1; 
    }
}