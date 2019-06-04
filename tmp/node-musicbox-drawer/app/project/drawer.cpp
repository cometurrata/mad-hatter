#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "http_client.h"
#include "project/knockdetection.h"

static Timer heartBeatTimer;

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(15 * 1000, [] { sendHeartBeat(); }).start();
}
