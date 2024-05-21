#include "MotorShield_PCA9685.h"

/*
If a Motordriver is connected, In this code we use an additional L298N Motorshield. 
We can use switch  use the PCA9685 with a config of a digital output and PWM driven channels.
We need a dutycycle from 0 to 100%.  So full range of 0 to 4095 is used to drive ENA pin (PWM controlled) and 2 pins IN1 and IN2 are used to set the direction of the motor
IN1 = HIGH and IN2 LOW,  forward.  IN1=LOW and IN2=HIGH reverse.  IN1 = IN2 = LOW, then stop.

test :  http://192.168.0.63/?pid=6000&KP=2&KI=0&KD=0
*/

/*
 * Set min and max values.
 * Set speed percent  0-4095


int minPower = 0;
int maxPower = 4095;

int IN1_Channel = 4;
int IN2_Channel = 5;
int ENA_Channel = 6; 

int IN3_Channel = 12;
int IN4_Channel = 13;
int ENB_Channel = 14;

*/

int Encoder1Pin = 14;

int desiredSpeed = 80; // Preset speed (0-160)rpm
float Kp = 100000; // Proportional constant
float Ki = 0; // Integral constant
float Kd = 0;
float integral = 0;
float previousError = 0;
unsigned long timeBetweenPulses;
float pulseTime = 0.0;
int Speed = 130; // Start with a midpoint value for the continuous rotation servo
String inputString = ""; // A string to store the serial input
bool stringComplete = false; // Whether the string is complete
int output = 0;

void processSerialInput(String input) {
  input.trim();
  char command = input.charAt(0);
  float value = input.substring(1).toFloat();

  switch (command) {
    case 'P':
      Kp = value;
      Serial.print("Kp set to: ");
      Serial.println(Kp);
      break;
    case 'I':
      Ki = value;
      Serial.print("Ki set to: ");
      Serial.println(Ki);
      break;
    case 'D':
      Kd = value;
      Serial.print("Kd set to: ");
      Serial.println(Kd);
      break;
    case 'S':
      desiredSpeed = value;
      Serial.print("Desired speed set to: ");
      Serial.println(desiredSpeed);
      break;
    case 'E':
      desiredSpeed = 0;
      break;
    default:
      Serial.println("Invalid command");
      break;
  }
}

/**
 * Drive forward
*/

int pidControl(MotorShield PWM, int S, float KP = 0.5, float KI = 0 , float KD = 0) {


  Kp = KP;
  Ki = KI;
  Kd = KD;

  //Serial.print("PIDControl Launched S P I D : ");
  //Serial.println(S + '   ' + Kp + '   ' + Ki + '   ' + Kd);

  //Serial.println("Min:0,Max:10000");

  desiredSpeed = S;

  int Pulse1Pin = 14;
  int i = 0;

  pinMode(Pulse1Pin, INPUT); // Enable the internal pull-up resistor for pin 9

  PWM.set_channel_value(IN3_Channel, 0 );
  PWM.set_channel_value(IN4_Channel, 4095);
  PWM.set_channel_value(ENB_Channel, 4095 );
  delay(100);
  PWM.set_channel_value(ENB_Channel, 0 );


  int counter = 0;

  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 0);  // Groene achtergrondverlichting (R,G,B)
  lcd.print("Starting PID");

  while (counter < 2000) {

    counter ++;

    // Read serial input if available
    while (Serial.available()) {
      char inChar = (char)Serial.read();
      if (inChar == '\n') {
        stringComplete = true;
      } else {
        inputString += inChar;
      }
    }

    if (stringComplete) {
      processSerialInput(inputString); // Process serial input
      inputString = ""; // Clear the string for the next input
      stringComplete = false; // Reset the flag
    }

    int resultValue = map(Speed, 0, 10000, minPower, maxPower);
    PWM.set_channel_value(ENB_Channel, resultValue ); 

    //delay(20);

    unsigned long durationLow = pulseIn(Pulse1Pin, HIGH, 500000);   
    durationLow = durationLow ;

    // Add the times of the HIGH and LOW periods to get the total time between two pulses
    pulseTime = ((durationLow)/1000.0);

    int measuredSpeed = 60/((pulseTime*20)/1000); // Measured speed in mm/sec

    if (measuredSpeed > 10000) {
      lcd.setCursor(0, 1);
      lcd.print("Motor error");     
      PWM.set_channel_value(ENB_Channel, minPower );
      return 0;
    }



    measuredSpeed = constrain(measuredSpeed, 0, 10000);



    int error = desiredSpeed - measuredSpeed;

    // Calculate the integral
    integral += error;

    // Calculate the derivative
    float derivative = error - previousError;

    // Calculate the PWM output using PID controller
    int output = Kp * error + Ki * integral + Kd * derivative;
    previousError = error;

    // Calculate the new speed and constrain it to the range of the servo
    Speed = constrain(Speed + output, 0, 10000); // Constrain to 0-90 for continuous rotation



    // Set the PWM value
    resultValue = map(Speed, 0, 10000, minPower, maxPower);
    PWM.set_channel_value(ENB_Channel, resultValue ); 

    // Display the speed and PWM value on the serial monitor
    Serial.print("Desired speed: ");
    Serial.print(desiredSpeed);
    Serial.print(" r/min\tMeasured speed: ");
    Serial.print(measuredSpeed);
    Serial.print(" pwm\tSpeed: ");
    Serial.println(Speed);    

    lcd.setCursor(0, 1);
    lcd.print(measuredSpeed);
    lcd.print(" tr/min");


    delay(5);
  } 

  PWM.set_channel_value(ENB_Channel, minPower );
  delay(200); 

  return 1;
}