#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "http_client.h"

static Timer heartBeatTimer;

uint32_t threshold = 15000;

bool uvstate = false;

Timer toggleTimer;

void toggle()
{
    uvstate = !uvstate;
    digitalWrite(5, uvstate);
    debugf("toggle %d", uvstate);
}

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(30 * 1000, sendHeartBeat);
    pinMode(5, OUTPUT);
    toggleTimer.initializeMs(5000, toggle).start();
}
