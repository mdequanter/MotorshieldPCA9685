/*  Code for using continuous rotation servos. Also called 360 degrees servo's.*/

#include "MotorShield_PCA9685.h"

int ServoCwMax = 200;
int ServoZero = 300; 
int ServoCcwMax= 400;


/**
 * Turn servo Clockwise
 * Set speed percent  0-100
 * int Channel 0-6
*/
int turnClockwise(MotorShield PWM, int channel, int speed) {
  int resultSpeed = map(speed, 0, 100, ServoZero, ServoCwMax);
  PWM.set_channel_value(channel, resultSpeed ); 
  return resultSpeed;
}

/**
 * Turn servo CounterClockwise
 * Set speed percent  0-100
 * int Channel 0-6
*/


int turnCounterClockwise(MotorShield PWM, int channel, int speed) {
  int resultSpeed = map(speed, 0, 100, ServoZero, ServoCcwMax);
  PWM.set_channel_value(channel, resultSpeed ); 
  return resultSpeed;
}

int testCCR(MotorShield PWM, int channel) {
  Serial.println("Testing CCR servo");
  int resultDegrees = 0;
  for (int i = 0; i<=100; i+=1) {
    resultDegrees = map(i, 0, 100, ServoCwMax, ServoCcwMax);
    PWM.set_channel_value(channel, resultDegrees);
    //Serial.println(resultDegrees);
    delay(15);
  }
  for (int i = 100; i>1; i--) {
    resultDegrees = map(i, 0, 100, ServoCwMax, ServoCcwMax);
    PWM.set_channel_value(channel, resultDegrees);
    //Serial.println(resultDegrees);
    delay(15);

  }

  for (int i = 0; i<=100; i++) {
    resultDegrees = map(i, 0, 100, ServoCcwMax, ServoZero);
    PWM.set_channel_value(channel, resultDegrees);
    //Serial.println(resultDegrees);
    delay(15);

  }




  return 1;

}
