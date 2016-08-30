#include "BMA250.h"
#include <inttypes.h>
#include "Arduino.h"
#include <Wire.h>

BMA250::BMA250()
{
}

void BMA250::begin(uint8_t range, uint8_t bw)
{
  //Setup the range measurement setting
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(0x0F); 
  Wire.write(range);
  Wire.endTransmission();
  //Setup the bandwidth
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(0x10);
  Wire.write(bw);
  Wire.endTransmission();
}


void BMA250::read()
{
  //Set register index
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(0x02);
  Wire.endTransmission();
  
  //Request seven data bytes
  Wire.requestFrom(BMA250_I2CADDR,7);
  
  //Receive acceleration measurements as 16 bit integers
  rawX = (int16_t)Wire.read();
  rawX |= (int16_t)Wire.read()<<8;
  rawY = (int16_t)Wire.read();
  rawY |= (int16_t)Wire.read()<<8;
  rawZ = (int16_t)Wire.read();
  rawZ |= (int16_t)Wire.read()<<8;
  
  //Only use the 10 significant bits
  rawX >>= 6; 
  rawY >>= 6; 
  rawZ >>= 6;
  
  //Receive temperature measurement
  rawTemp = Wire.read();
}

float BMA250::X()
{
  return rawX / 256.0;
}

float BMA250::Y()
{
  return rawY / 256.0;
}

float BMA250::Z()
{
  return rawZ / 256.0;
}

int16_t BMA250::Temp()
{
  return rawTemp;
}

float BMA250::Roll()
{
  return atan2(Y(), Z()) / (2 * M_PI);
}

float BMA250::Pitch()
{
  return atan2(-X(), sqrt(Y()*Y() + Z()*Z())) / (1.5);
}

