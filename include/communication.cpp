/*
Module to communicate with the world
*/

const int bufferSize = 1024;

/**
 * Send to serial
 */
bool sendSensorData() {
    float distance = getDistance();
    Serial.println("distance-" + String(distance));
    float temperature = getTemperature();
    Serial.println("temperature-" + String(temperature));
    return true;
}


void readCommandsFromSerial() {
  // Controleer of er gegevens beschikbaar zijn om te lezen van de seriële poort
  if (Serial.available() > 0) {
        // Maak een buffer aan om de ontvangen gegevens op te slaan
        char jsonBuffer[bufferSize];
        // Lees de gegevens van de seriële poort en sla ze op in de buffer
        int bytesRead = Serial.readBytesUntil('\n', jsonBuffer, bufferSize);
        // Voeg een null-terminator toe aan het einde van de buffer
        jsonBuffer[bytesRead] = '\0';
        
        // Maak een JSON-object aan om de ontvangen gegevens te parsen
        DynamicJsonDocument jsonDocument(bufferSize);
        // Parse de ontvangen JSON-gegevens
        DeserializationError error = deserializeJson(jsonDocument, jsonBuffer);

        // Controleer of de parsing succesvol was
        if (error) {
            Serial.print("JSON parsing failed: ");
            Serial.println(error.c_str());
            return;
        }

        // Haal de waarden op uit het JSON-object
        String direction = jsonDocument["direction"];
        int speed = jsonDocument["speed"];
        int servoPosition = jsonDocument["servoPosition"];
        String trafficLight = jsonDocument["trafficLight"];


        if (direction == "vooruit") {
            forward(PWM, speed);
        }
        if (direction == "achteruit") {
            backward(PWM, speed);
        }
        if (direction == "links") {
            turnleft(PWM, speed);
        }
        if (direction == "rechts") {
            turnright(PWM, speed);
        }    
        if (direction == "stop") {
            stop(PWM);
        }    

            
        setDegrees(PWM, 0, servoPosition);
        setDegrees(PWM, 1, servoPosition);

        //  'trafficLight' parameter (RED on channel 8,  Orange on channel 9 and green on channel 10)
        if (trafficLight == "red") {
            setLed(PWM,8,0);
            setLed(PWM,9,0);
            setLed(PWM,10,100);
        }
        if (trafficLight == "green") {
            setLed(PWM,8,100);
            setLed(PWM,9,0);
            setLed(PWM,10,0);
        }
        if (trafficLight == "orange") {
            setLed(PWM,8,0);
            setLed(PWM,9,100);
            setLed(PWM,10,0);
        }
        
        Serial.println("SET: Direction: " + direction + ", Speed: " + String(speed) + ", Servo Position: " + String(servoPosition) + ", Light: " + trafficLight);

    }
}