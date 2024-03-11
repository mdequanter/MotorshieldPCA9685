#include "MotorShield_PCA9685.h"

/*
LED can be used also with 4096 values at 50hz,  We need a dutycycle from 0 to 100%.  So full range of 0 to 4096 is used.
*/



int ledMin = 0; 
int ledMax= 4095;


/**
 * PWM control a LED.
 * Set speed percent  0-100
*/


int setLed(MotorShield PWM, int channel, int value) {
  int resultValue = map(value, 0, 100, ledMin, ledMax);
  PWM.set_channel_value(channel, resultValue ); 
  return resultValue;
}


int testLed(MotorShield PWM, int channel) {
  Serial.println("Testing LED servo");
  int resultValue = 0;
  for (int i = 0; i<=100; i+=1) {
    resultValue = map(i, 0, 100, ledMin, ledMax);
    PWM.set_channel_value(channel, resultValue);
    delay(200);
  }
  for (int i = 100; i>1; i--) {
    resultValue = map(i, 0, 100, ledMin, ledMax);
    PWM.set_channel_value(channel, resultValue);
    delay(200);
  }

  return 1;

}