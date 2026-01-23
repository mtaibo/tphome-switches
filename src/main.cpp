# include <Arduino.h>

const int pins[] = {0, 2, 4, 5, 12, 13, 14, 15, 16};
const int numPins = 9;

void setup() {

  // Start communication
  Serial.begin(115200);
  
  // 20 seconds delay to give time for the user to start 
  // the monitor and see how the test goes
  for (int i = 0; i < 20; i++) {
    Serial.print(20 - i); 
    Serial.println(" segundos restantes para el inicio...");
    delay(1000);
  }
}

void loop() {

  // Begining of the chip main loop
  Serial.println("Iniciando el bucle de los 9 pines...");
  delay(3000);

  // Loop that goes through every pin on the pin list
  for (int i = 0; i < numPins; i++) {

    int pin = pins[i]; 

    // Test message at the begining of the testing of each pin
    Serial.print("Testeando el GPIO: "); Serial.println(pin);
    delay(5000);

    pinMode(pin, OUTPUT);
    
    // Loop that repeats three times at each pin to 
    // have time and confirm which pin is turning on and off
    for (int j = 0; j < 3; j++) {

      digitalWrite(pin, HIGH);
      Serial.print("Pin "); Serial.print(pin); Serial.println(" en estado HIGH...");
      delay(3000); 

      digitalWrite(pin, LOW);
      Serial.print("Pin "); Serial.print(pin); Serial.println(" en estado LOW...");
      delay(3000);

    }
    
    pinMode(pin, INPUT);
  }

  // Ending of the chip main loop with a 5 seconds delay
  Serial.println("Finalizado el bucle de los 8 pines.");
  delay(5000);
}