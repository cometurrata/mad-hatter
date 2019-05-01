#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "LSM9DS1.h"

#define I2C_AXL_GYR_ADDRESS (0x6b)
#define I2C_MAGNETOMETER_ADDRESS (0x1e)

// class default I2C address is 0x1E
// specific I2C addresses may be passed as a parameter here
// this device only supports one I2C address (0x1E)
int16_t mx, my, mz;
LSM9DS1 imu;
Timer procTimer;
Timer imuTaskTimer;

#define LSM9DS1_INT 4 // Needs to be an interrupt pin

uint32_t val = 1;

void imuTask()
{
    imu.readMag();

    uint32_t tmp = abs(imu.mz);

    val = (tmp * 3 + val * 7) / 10;
}

uint32_t clockGetVal()
{
    return val;
}

void clockInit()
{
    Wire.pins(4, 5); // SDA, SCL

    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // I2C frequency at 400 kHz

    imu.init(IMU_MODE_I2C, LSM9DS1_AG_ADDR(1), LSM9DS1_M_ADDR(1));
    if (0 == imu.begin())
    {
        Serial.println("Failed to communicate with LSM9DS1.");
    }

    imu.setMagScale(2);
    imu.setMagODR(7);

    procTimer.initializeMs(20, imuTask).start();
}
