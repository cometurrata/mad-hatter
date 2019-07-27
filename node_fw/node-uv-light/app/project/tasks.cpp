#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "http_client.h"

static Timer heartBeatTimer;

uint32_t threshold = 15000;

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(30 * 1000, sendHeartBeat);
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
}
