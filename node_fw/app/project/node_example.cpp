#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "common/http_client.h"

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
   sensorTimer.initializeMs(3000, temperatureSensorChanged).start();
}