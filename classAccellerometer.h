#ifndef classAccellerometer
#define classAccellerometer

class Accellerometer
{
  public:
    Accellerometer(int port);
    void initialize();
    char displayOrientation();
    int checkAngles();

  private:
    int AcX, AcY, AcZ;
    char previousOrientation;
    #define MPU_addr 0x68 /* MPU6050 address */
    #define ACCEL_XOUT_H 0x3B /* AcX most significant bit address */
};

#endif