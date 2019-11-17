#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "gesturesensor.h"

static Timer sensorTimer;
static Timer heartBeatTimer;

Timer togglePinTimer;


uint8_t state = 0;

void togglePinTask()
{
    digitalWrite(14, state);
    state = !state;
    debugf("pin 14 : %d ", state);
}

void temperatureSensorChanged(void)
{
    sendNodeUpdate("{"
                   "sensor:temperature"
                   "value:13"
                   "}");
}

void nodeExampleInit(void)
{
    pinMode(14, OUTPUT);
    digitalWrite(14, LOW);
    togglePinTimer.initializeMs(4000, &togglePinTask).start();
    // GestureSensor.init();
    // sensorTimer.initializeMs(3 * 1000, temperatureSensorChanged).start();
}

void nodeHeartBeatInit()
{
    // heartBeatTimer.initializeMs(30 * 1000, sendHeartBeat);
}
