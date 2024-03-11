#include "MotorShield_PCA9685.h"

/*

SG90 180 degrees servo's usually have a duty cycle between 0.5ms and 2.5ms
With 4096 values at 50hz,  100 corresponds with 0.5ms and 510 corresponds with 2.5ms


These min and max values can vary from servo to servo.  
Measure the exact values to get a range of 180 degrees
*/

int ServoMin = 110; 
int ServoMax= 500;



/**
 * Turn servo Clockwise
 * Set speed percent  0-100
 * int Channel 0-6
*/
int setDegrees(MotorShield PWM, int channel, int degrees) {
  int resultDegrees = map(degrees, 0, 180, ServoMin, ServoMax);
  PWM.set_channel_value(channel, resultDegrees ); 
  return resultDegrees;
}


int testServo(MotorShield PWM, int channel) {
  Serial.println("Testing SG90 servo");
  int resultDegrees = 0;
  for (int i = 0; i<=180; i+=1) {
    resultDegrees = map(i, 0, 180, ServoMin, ServoMax);
    PWM.set_channel_value(channel, resultDegrees);
    delay(15);
  }
  for (int i = 180; i>1; i--) {
    resultDegrees = map(i, 0, 180, ServoMin, ServoMax);
    PWM.set_channel_value(channel, resultDegrees);
    delay(15);
  }

  return 1;

}