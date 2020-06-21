#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"

static Timer sensorTimer;

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
