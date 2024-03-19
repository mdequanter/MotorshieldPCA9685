#include <SPI.h>
#include <LoRa.h>

int counter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {

  String data = "";

  if (Serial.available()) {
    String data = Serial.readStringUntil('\n'); // Lees de seriële ingang tot een newline-teken
    // send packet
    LoRa.beginPacket();
    LoRa.print(data);
    LoRa.endPacket();
    Serial.print(data);
    Serial.println("   --> Data was send");  
 }
   delay(20);
}
