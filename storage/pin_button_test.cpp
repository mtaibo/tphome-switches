#include <Arduino.h>

int pins[] = {4, 5, 12, 13, 15};
int pinsNum = 5;

void setup() {

  Serial.begin(115200);

  // Set every pin to INPUT mode to prevent any problem
  for (int i = 0; i < pinsNum; i++) {
    pinMode(pins[i], INPUT); 
  }
}

void loop() {

  // Loop that goes through every pin, check its status and show it
  for (int i = 0; i < pinsNum; i++) {
    int val = digitalRead(pins[i]);
    Serial.printf("PIN %d:%d | ", pins[i], val);
  }

  // Print and delay for a proper reading
  Serial.println(); 
  delay(150);
}