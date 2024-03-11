#include "MotorShield_PCA9685.h"

/*
If a Motordriver is connected, In this code we use an additional L298N Motorshield. 
We can use switch  use the PCA9685 with a config of a digital output and PWM driven channels.
We need a dutycycle from 0 to 100%.  So full range of 0 to 4095 is used to drive ENA pin (PWM controlled) and 2 pins IN1 and IN2 are used to set the direction of the motor
IN1 = HIGH and IN2 LOW,  forward.  IN1=LOW and IN2=HIGH reverse.  IN1 = IN2 = LOW, then stop.
*/



/**
 * Set min and max values.
 * Set speed percent  0-4095
*/

int minPower = 0;
int maxPower = 4095;


int IN1_Channel = 4;
int IN2_Channel = 5;
int ENA_Channel = 6; 


/**
 * Drive forward
*/

int forward(MotorShield PWM, int value) {
  PWM.set_channel_value(IN1_Channel, 0 );
  PWM.set_channel_value(IN2_Channel, 4095);
  int resultValue = map(value, 0, 100, minPower, maxPower);
  PWM.set_channel_value(ENA_Channel, resultValue ); 
  return resultValue;
}



/**
 * drive backward
*/

int backward(MotorShield PWM, int value) {
  PWM.set_channel_value(IN1_Channel, 4095 );
  PWM.set_channel_value(IN2_Channel, 0);
  int resultValue = map(value, 0, 100, minPower, maxPower);
  PWM.set_channel_value(ENA_Channel, resultValue ); 
  return resultValue;
}


/**
 * Stop motor
*/

int stop(MotorShield PWM) {
  PWM.set_channel_value(IN1_Channel, 0 );
  PWM.set_channel_value(IN2_Channel, 0);
  PWM.set_channel_value(ENA_Channel, 0);
  return 1;
}
