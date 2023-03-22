#include "classAccellerometer.h"
#include "Arduino.h"
#include <Wire.h>

Accellerometer::Accellerometer(int port)
{

}

void Accellerometer:: initialize()
{
  Wire.begin(); /* Join the bus */
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x6B); /* Register the address */
  Wire.write(0); /* Write the data to the desired address */
  Wire.endTransmission(true);
}

int Accellerometer::checkAngles()
{
  Wire.beginTransmission(MPU_addr); /* Join the I2C bus with the MPU address*/
  Wire.write(ACCEL_XOUT_H); /* queue the bytes of data needed for transmission*/
  Wire.endTransmission(true);
  Wire.requestFrom(MPU_addr,6,true);
  AcX = Wire.read()<<8|Wire.read();
  AcY = Wire.read()<<8|Wire.read();
  AcZ = Wire.read()<<8|Wire.read();
}

char Accellerometer:: displayOrientation()
{
  if(AcY >= 14000 && AcY <= 16000)
  {
    previousOrientation = 'l';
    return 'l';
  }
  else if(AcY <= -14000 && AcY >= -16000)
  {
    previousOrientation = 'r';
    return 'r';
  }
  else if(AcX >= 16000)
  {
    previousOrientation = 'U';
    return 'U';
  }
  else if(AcX <= -14000 && AcX >= -16000)
  {
    previousOrientation='L';
    return 'L';
  }
  else if((AcX >= -5000 && AcX <= 9000) | (AcX >= 700 && AcX <= 500))
  {
    previousOrientation = 'F';
    return 'F';
  }
  else
  {
    return previousOrientation;
  }
}