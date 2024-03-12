#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>



#include "MotorShield_PCA9685.h"  // Custom designed library to control PCA9685 (16-channel, 12-bit PWM Fm+ I2C-bus LED controller)
#include "CCR.cpp"   // continuous rotation servo controls methods
#include "SG90.cpp"  // standard 90 to 180 degrees servo control
#include "LED.cpp"   // Using LED's with PWM brightness control
#include "SWITCH.cpp" // Using channels as a switch (100% duty cycle)
#include "MOTOR.cpp" //  Methods to use L298N 2 channel H-Bridge motor shield
#include "webcontent.cpp" // Html content of web interface
#include "HCSR04.cpp"



const char* ssid = "DUCK";
const char* password = "DUCK123$";

WebServer server(80);
MotorShield PWM;

int MIN_VALUE = 0;


void handleDistance() {

  float distance =  0.00;
  distance = getDistance();
  String webContent = "<!DOCTYPE html><html><head><title>Afstandssensor</title>";
  webContent += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  //webContent += "<meta http-equiv='refresh' content='1'>"; // Herlaad elke seconde
  webContent += "</head><body style='text-align: center;'>";
  webContent += "<h1>Ultrasonic sensor</h1>";
  webContent += "<p>Distance: ";
  webContent += String(distance);
  webContent += " cm</p>";
  webContent += "</body></html>";

  server.send(200, "text/html", webContent );

  unsigned long startTime = millis();
  unsigned long duration = 20000;

  while (true) {

    distance = getDistance();  

    float mappedValue = map(distance, 0, 20, 10, 40);

    if (distance <=10.0) {
      setLed(PWM,8,0);
      setLed(PWM,9,0);
      setLed(PWM,10,100);
 
    }
    if (distance >10.1 and distance <=30.0) {
      setLed(PWM,8,0);
      setLed(PWM,9,100);
      setLed(PWM,10,0);
    }
    if (distance >30.1) {
      setLed(PWM,8,100);
      setLed(PWM,9,0);
      setLed(PWM,10,0);
    }

    if (distance > 10.0) {

      setDegrees(PWM, 0, 89-mappedValue);
      delay(15);
      setDegrees(PWM, 1, 89+mappedValue);
    }

    delay(25);


    if (millis() - startTime >= duration) {
    // Stop de procedure na 20 seconden
      setDegrees(PWM, 0, 89);
      setDegrees(PWM, 1, 89);      

      return;
    }

  }

}




void handleRoot() { 
  // 'speed' parameter
  if (server.hasArg("speed") and  server.hasArg("direction")) {

    int speed = server.arg("speed").toInt();
    if (server.arg("direction") == "vooruit") {
      forward(PWM, speed);
    }
    if (server.arg("direction") == "achteruit") {
      backward(PWM, speed);
    }
  }
      
  // 'servoPosition' parameter
  if (server.hasArg("servoPosition")) {
    int position = server.arg("servoPosition").toInt();
    setDegrees(PWM, 0, position);
    setDegrees(PWM, 1, position);
  }

  //  'trafficLight' parameter (RED on channel 8,  Orange on channel 9 and green on channel 10)
  if (server.hasArg("trafficLight")) {
    String color = server.arg("trafficLight");
    if (color == "red") {
      Serial.println("Switch Red Light ON");
      setLed(PWM,8,0);
      setLed(PWM,9,0);
      setLed(PWM,10,100);
    }
    if (color == "green") {
      Serial.println("Switch green Light ON");
      setLed(PWM,8,100);
      setLed(PWM,9,0);
      setLed(PWM,10,0);
    }
    if (color == "orange") {
      Serial.println("Switch orange Light ON");
      setLed(PWM,8,0);
      setLed(PWM,9,100);
      setLed(PWM,10,0);
    }
  }

  server.send(200, "text/html", htmlContent);
}




void setup() {
  Serial.begin(115200);
  if(PWM.begin()) {
    PWM.init(MIN_VALUE);
  }
  PWM.set_hz(50);


 WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  

  setLed(PWM,8,0);
  setLed(PWM,9,0);
  setLed(PWM,10,0);

  setDegrees(PWM, 0, 89);
  setDegrees(PWM, 1, 89);



  int blink = 100;
  for (int i = 0; i<=20; i++) {
    setLed(PWM,8,blink);
    if (blink == 100) {
      blink = 0;
    } else  {
      blink = 100;
    }
    delay(250);
  }
  server.on("/", handleRoot);

  server.on("/distance", handleDistance);
  
  server.begin();

  pinMode(12, OUTPUT);// Set de trigPin als een uitgang
  pinMode(13, INPUT); // Set de echoPin als een ingang

  
}


void loop() {

  server.handleClient();
  delay(2);

}



