#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Wire.h>
#include <HMC5883L.h>

HMC5883L compass;

// class default I2C address is 0x1E
// specific I2C addresses may be passed as a parameter here
// this device only supports one I2C address (0x1E)
int16_t mx, my, mz;
Timer procTimer;

uint32_t val = 1;

void task()
{
    compass.read(&mx, &my, &mz);
    uint32_t tmp = abs(mz);
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

    if (!compass.begin())
    {
        Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    }
    compass.setRange(HMC5883L_RANGE_1_3GA);
    compass.setMeasurementMode(HMC5883L_CONTINOUS);
    compass.setDataRate(HMC5883L_DATARATE_75HZ);
    compass.setSamples(HMC5883L_SAMPLES_8);
    procTimer.initializeMs(20, task).start();
}
