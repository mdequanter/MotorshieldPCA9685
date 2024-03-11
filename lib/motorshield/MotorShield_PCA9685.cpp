/**
@file MotorShield_PCA9685.cpp
@brief This library is designed for driving motor shields and servos via I2C as part of the Integrated Practicum course.

Released under APACHE LICENSE, VERSION 2.0

http://www.apache.org/licenses/

@author Maarten Dequanter (maarten.dequanter@gmail.com)
*/


#include "MotorShield_PCA9685.h"

#define I2C_SDA 14
#define I2C_SCL 15


/**
 @brief Constructor
*/
MotorShield::MotorShield(uint8_t addr){
  _i2caddr = addr;
  Wire.begin(I2C_SDA, I2C_SCL);  // On the ESP32-CAM , available I2C pins are different.  Romove params I2C_SDA, I2C_SCL for a standard setup. 
}

/**
 @brief Begin Device
 @retval true normaly done
 @retval false device error
*/
bool MotorShield::begin()
{
  if(checkI2c(0x00)) {
    Serial.println("PCA9685 shield active");
    return true;
  } else{
    Serial.println("PCA9685 shield not found or an error orrured. Check power and IC2 pins");
    return  false;
  }
}

/**
 @brief Set Config
*/
void MotorShield::init(uint8_t value)
{
  writeI2c(PCA9685_ALL_LED_ON_L_REG, 0x00);
  writeI2c(PCA9685_ALL_LED_ON_H_REG, 0x00);
  writeI2c(PCA9685_ALL_LED_OFF_L_REG, value & 0xFF);   // most significant byte
  writeI2c(PCA9685_ALL_LED_OFF_L_REG, value >> 8);    // least significant byte

  writeI2c(PCA9685_MODE2_REG, PCA9685_OUTDRV);
  writeI2c(PCA9685_MODE1_REG, PCA9685_ALLCALL);
  delay(100);

  uint8_t buffer[1];
  readI2c(PCA9685_MODE1_REG, 1, buffer);
  uint8_t mode = buffer[0] & ~PCA9685_SLEEP;
  writeI2c(PCA9685_MODE1_REG, mode);
  delay(100);
}


/**
 @brief Return dutyCycle in percent.
 @param [in] uint16_t value.
*/ 
float calculateDutyCycle(uint16_t value) {
    return (float)value / 4095.0f * 100.0f;
}

/**
 @brief Return dutyCycle active time in ms.
 @param [in] uint16_t value.
*/ 
float calculateActiveTimeMs(uint16_t value, uint16_t hz = 50) {
    float periodMs = 1000.0f / hz; // Periode in ms
    float dutyCycle = (float)value / 4095.0f; // Duty cycle als fractie
    return periodMs * dutyCycle; // Actieve tijd in ms
}


/**
 @brief convert value hz to prescale.
 @param [in] hz hz.
*/
uint16_t MotorShield::calc_prescale(uint16_t hz){
  return uint16_t(round(PCA9685_OSC_CLOCK/4096/hz)-1);
}

/**
 @brief convert value prescale to hz.
 @param [in] hz hz.
*/
void MotorShield::calc_hz(uint16_t prescale, uint16_t *value) {
  uint8_t d = 0;
  if (((prescale +1) % 2) == 0) {
      d = 0.5;
  }
  else {
      d = 4.999999e-1;
  }
  value[0] = uint8_t(PCA9685_OSC_CLOCK/4096/(prescale+1+d)); // min hz
  value[1] = uint8_t(PCA9685_OSC_CLOCK/4096/(prescale+1-d)); // max hz
}


/**
 @brief set hz.
 @param [in] hz hz.
*/
void MotorShield::set_hz(uint16_t hz) {
  uint16_t prescale = calc_prescale(hz);
  uint8_t buffer[1];
  readI2c(PCA9685_MODE1_REG, 1, buffer);
  uint8_t oldmode = buffer[0];
  uint8_t newmode = oldmode | PCA9685_SLEEP;
  writeI2c(PCA9685_MODE1_REG, newmode);
  writeI2c(PCA9685_PRE_SCALE_REG, prescale);
  //printf("Prescale Hz: %.2f ms\n", prescale);  
  writeI2c(PCA9685_MODE1_REG, oldmode);
  delay(100);
  uint8_t restart = oldmode | PCA9685_RESTART;
  writeI2c(PCA9685_MODE1_REG, restart);
}

/**
 @brief Write value to channel.
 @param [in] channel channel of pwm.
 @param [in] value value of pwm.
*/
void MotorShield::set_channel_value(uint8_t channel, uint16_t value){
  writeI2c(PCA9685_LED0_ON_L_REG+channel*4, 0x00);
  writeI2c(PCA9685_LED0_ON_H_REG+channel*4, 0x00);
  // Calculate and print the duty cycle for debugging
  //float dutyCycle = calculateDutyCycle(value);
  //Serial.print("Duty Cycle: ");
  //Serial.print(dutyCycle);
  //Serial.println("%");  
  //float activeTimeMs = calculateActiveTimeMs(value);
  //printf("Actieve tijd: %.2f ms\n", activeTimeMs);  
  writeI2c(PCA9685_LED0_OFF_L_REG+channel*4, (value & 0xFF));
  writeI2c(PCA9685_LED0_OFF_H_REG+channel*4, (value >> 8));
}

/**
 @brief get value to channel.
 @param [in] channel channel of pwm.
 @param [our] value value of channel.
*/
uint16_t MotorShield::get_channel_value(uint8_t channel){
  uint8_t msb[1];
  uint8_t lsb[1];
  readI2c((PCA9685_LED0_OFF_L_REG+channel*4), 1, lsb);
  readI2c((PCA9685_LED0_OFF_H_REG+channel*4), 1, msb);
  uint16_t value = ((msb[0] & 0xFF)<<8) + (lsb[0] & 0xFF);
  return value;
}

/**
 @brief Write I2C Data
 @param [in] register_addr Write Register Address
 @param [in] value Write Data
*/
void MotorShield::writeI2c(uint8_t register_addr, uint8_t value) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(register_addr);
  Wire.write(value);
  Wire.endTransmission();
}

/**
 @brief Read I2C Data
 @param [in] register_addr register address
 @param [in] num Data Length
 @param [out] *buf Read Data
*/
void MotorShield::readI2c(uint8_t register_addr, int num, uint8_t *buf) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(register_addr);
  Wire.endTransmission();
  Wire.requestFrom(_i2caddr, num);

  int i = 0;
  while (Wire.available())
  {
    buf[i] = Wire.read();
    i++;
  }
}

/**
 @brief Check I2C
 @param [in] register_addr register address
 @param [out] boolean true or false.
*/
boolean MotorShield::checkI2c(uint8_t register_addr) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(register_addr);
  Wire.endTransmission();
  uint8_t result = Wire.requestFrom(_i2caddr, 1);
  if(result == 1) {
    return true;
  } else {
    return false;
  }
}
