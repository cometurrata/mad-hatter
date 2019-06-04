#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Wire.h>
#include "MechaQMC5883.h"

MechaQMC5883 qmc;

int mx, my, mz;
Timer procTimer;

uint32_t val = 1;

void task()
{
    qmc.read(&mx, &my, &mz);
    uint32_t tmp = abs(mz);
    val = (tmp * 5 + val * 95) / 100;
    debugf("%d ", val);
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
    qmc.init();
    qmc.setMode(Mode_Continuous, ODR_200Hz, RNG_2G, OSR_256);
    procTimer.initializeMs(20, task).start();
}
