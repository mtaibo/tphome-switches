#include <Arduino.h>

// Definición de pines (basado en tus hallazgos)
const int BTN_SUP = 12;
const int BTN_INF = 5;

const int LED_SUP = 16;
const int LED_INF = 2;

const int RELE_A = 15;
const int RELE_B = 13;

void setup() {
  // Configuración de LEDs y Relés
  pinMode(LED_SUP, OUTPUT);
  pinMode(LED_INF, OUTPUT);
  pinMode(RELE_A, OUTPUT);
  pinMode(RELE_B, OUTPUT);

  // Configuración de Botones con PULLUP (por seguridad extra)
  pinMode(BTN_SUP, INPUT_PULLUP);
  pinMode(BTN_INF, INPUT_PULLUP);
}

void loop() {
  // Lógica para el botón Superior -> Controla LED 16 y Relé 4
  if (digitalRead(BTN_SUP) == LOW) {
    digitalWrite(LED_SUP, HIGH);
    digitalWrite(RELE_A, HIGH);
  } else {
    digitalWrite(LED_SUP, LOW);
    digitalWrite(RELE_A, LOW);
  }

  // Lógica para el botón Inferior -> Controla LED 2 y Relé 15
  if (digitalRead(BTN_INF) == LOW) {
    digitalWrite(LED_INF, HIGH);
    digitalWrite(RELE_B, HIGH);
  } else {
    digitalWrite(LED_INF, LOW);
    digitalWrite(RELE_B, LOW);
  }
}