#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Wire.h>
#include "MechaQMC5883.h"

MechaQMC5883 qmc;

int mx, my, mz;
Timer procTimer;

int32_t val = 1;

int pastWeight = 100;
int presentWeight = 3;

void task()
{
    static int i = 0;
    qmc.read(&mx, &my, &mz);
    int32_t tmp = mz;
    val = (tmp * presentWeight + val * pastWeight) / (pastWeight + presentWeight);
    if (!((i++) % 10))
    {
        debugf("%d ", val);
    }
}

int32_t clockGetVal()
{
    return val;
}

void clockInit()
{
    Wire.pins(4, 5); // SDA, SCL
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // I2C frequency at 400 kHz
    qmc.init();
    qmc.setMode(Mode_Continuous, ODR_200Hz, RNG_2G, OSR_256);
    procTimer.initializeMs(20, task).start();
}
