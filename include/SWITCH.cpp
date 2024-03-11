#include "MotorShield_PCA9685.h"

/*
Using switches can be achieved by setting duty cycle to 100% or 0. 4095 is ON and 0 is OFF
*/

/**
 * Turn on or Off a channel
*/
int setLed(MotorShield PWM, int channel, bool value = true) {
  if (value == true) {
    PWM.set_channel_value(channel, 4095); 
  } else {
    PWM.set_channel_value(channel, 0);
  }
  return 1;
}


int testSwitch(MotorShield PWM, int channel) {
  Serial.println("Testing Switch servo");
  int value = 0;
  for (int i = 0; i<=20; i+=1) {
    PWM.set_channel_value(channel, value);
    
    if (value == 0) {
      value = 4065;
    } else {
      value = 0;
    }

    delay(250);
  }
  return 1;
}