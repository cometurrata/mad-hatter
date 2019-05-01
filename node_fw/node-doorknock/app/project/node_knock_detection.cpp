#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "http_client.h"
#include "project/knockdetection.h"

static Timer sensorTimer;
static Timer heartBeatTimer;

bool infoSent;

void sendKnockDetection(void)
{
    if (getKnockDetected())
    {
        if (!infoSent)
        {
            infoSent = true;
            sendNodeUpdate("knockdetected");
        }
    }
    else
    {
        infoSent = false;
    }
}

void nodeKnockDetectionInit(void)
{
    knockDetectionInit();
    sensorTimer.initializeMs(3 * 1000, sendKnockDetection).start();
}

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(30 * 1000, [] { sendHeartBeat(); });
}
