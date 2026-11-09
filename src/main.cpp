#include <Arduino.h>

#define LED_PIN 4

void setup() {
  // Iniciamos la comunicación serial a la velocidad configurada
  Serial.begin(115200);
  
  // Configuramos el pin del LED como salida
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("¡ESP32 Iniciado!");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // Enciende el LED
  Serial.println("LED Encendido");
  delay(1000);                  // Espera 1 segundo (1000 ms)
  
  digitalWrite(LED_PIN, LOW);   // Apaga el LED
  Serial.println("LED Apagado");
  delay(1000);                  // Espera 1 segundo
}