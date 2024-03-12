#include "MotorShield_PCA9685.h"


/*
Ultrasolic detector  


*/

const int trigPin = 12;
const int echoPin    = 13;


long duration;          // Variabele om de tijd van het signaal te bewaren
int distance;           // Variabele om de berekende afstand te bewaren



/**
 * Get distance
 */
float getDistance() {
    // Stuur een 10µs puls naar de HC-SR04 om een meting te starten
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Meet de duur van het echo signaal
  duration = pulseIn(echoPin, HIGH);

  Serial.println(duration);
  // Bereken de afstand in centimeters
  distance = duration * 0.034 / 2;

  // Toon de afstand op de seriële monitor
  Serial.print("Afstand: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;  
}