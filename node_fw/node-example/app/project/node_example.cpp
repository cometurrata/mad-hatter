#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "http_client.h"

static Timer sensorTimer;
static Timer heartBeatTimer;

void temperatureSensorChanged(void)
{
    sendNodeUpdate("{"
                   "sensor:temperature"
                   "value:13"
                   "}");
}

void nodeExampleInit(void)
{
    sensorTimer.initializeMs(3 * 1000, temperatureSensorChanged).start();
}

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(30 * 1000, sendHeartBeat);
}
