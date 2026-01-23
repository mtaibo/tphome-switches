#include <Arduino.h>

// El LED interno suele estar en el GPIO 2
// Si tu módulo tiene otro LED externo, cambia el 2 por el número de pin
const int LED_PIN = 2; 

void setup() {
  // Inicializamos el pin como salida
  pinMode(LED_PIN, OUTPUT);
  
  // Abrimos el monitor serie para ver que el chip "está vivo"
  Serial.begin(115200);
  Serial.println("");
  Serial.println("--- PRUEBA DE HARDWARE INICIADA ---");
}

void loop() {
  Serial.println("Encendiendo LED...");
  digitalWrite(LED_PIN, LOW);  // En muchos ESP8266, LOW es encender
  delay(3000);                 // Espera 1 segundo
  
  Serial.println("Apagando LED...");
  digitalWrite(LED_PIN, HIGH); // HIGH es apagar
  delay(3000);                 // Espera 1 segundo
}