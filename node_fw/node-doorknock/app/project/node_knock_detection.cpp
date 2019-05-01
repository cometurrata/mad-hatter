#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "http_client.h"
#include "project/knockdetection.h"

static Timer sensorTimer;
static Timer heartBeatTimer;

bool infoSent = false;

void sendKnockDetection(void)
{
    if (getKnockDetected())
    {
        // debugf("Knock Detected");
        if (!infoSent)
        {
            infoSent = true;
            sendNodeUpdate("event");
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
    pinMode(0, OUTPUT);
    digitalWrite(0, HIGH);
    sensorTimer.initializeMs(100, sendKnockDetection).start();
}

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(30 * 1000, [] { sendHeartBeat(); }).start();
}
