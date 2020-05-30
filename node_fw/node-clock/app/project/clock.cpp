#include <SmingCore.h>
#include <Wire.h>
#include "MechaQMC5883.h"

Timer procTimer;

MechaQMC5883 qmc1, qmc2;

int mx1, my1, mz1;
int mx2, my2, mz2;

int32_t val_hand_1, val_hand_2 = 1;

bool pos_1_ok, pos_2_ok = false;

void task()
{
    static int i = 0;

    // First I2C bus
    Wire.pins(4, 5); // SDA, SCL
    Wire.begin();
    Wire.setClock(400000); // I2C frequency at 400 kHz

    qmc1.read(&mx1, &my1, &mz1);
    val_hand_1 = sqrt(mx1*mx1 + my1*my1 + mz1*mz1);

    // Second I2C bus
    Wire.pins(13, 12); // SDA, SCL
    Wire.begin();
    Wire.setClock(400000); // I2C frequency at 400 kHz

    qmc2.read(&mx2, &my2, &mz2);
    val_hand_2 = sqrt(mx2*mx2 + my2*my2 + mz2*mz2);

    int32_t threshold_1 = 14000;
    int32_t threshold_2 = 14000;

    // if (val_hand_1 > threshold) {
    //     debugf("UNO");
    // }

    // if (val_hand_2 > threshold) {
    //     debugf("DOS");
    // }

    #ifdef VERBOSE
        //debugf("val_hand_1: %d \t val_hand_2: %d", val_hand_1, val_hand_2);
    #endif

    pos_1_ok = (val_hand_1 > threshold_1) ? true : false;
    pos_2_ok = (val_hand_2 > threshold_2) ? true : false;

    #ifdef VERBOSE
        if (pos_1_ok) {
            debugf("pos_1_ok");
        }
        else {
            //debugf("val_hand_1: %d ", val_hand_1);
        }
        if (pos_2_ok) {
            debugf("pos_2_ok");
        }
    #endif

}

bool clockGetVal()
{
    return pos_1_ok && pos_2_ok;
}

void clockInit()
{   
    Wire.pins(4, 5); // SDA, SCL
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // I2C frequency at 400 kHz
    qmc1.init();
    qmc1.setMode(Mode_Continuous, ODR_200Hz, RNG_8G, OSR_256);

    Wire.pins(13, 12); // SDA, SCL
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // I2C frequency at 400 kHz
    qmc2.init();
    qmc2.setMode(Mode_Continuous, ODR_200Hz, RNG_8G, OSR_256);
    
    procTimer.initializeMs(30, task).start();
}
