#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "musicbox.h"
#include "tasks.h"

Node musicBoxNode;
static Timer sensorTimer;

void sensorTask()
{
    if (MusicBox.isMelodyCorrect())
    {
        Serial.println("Melody is solved !!");
        musicBoxNode.setSolved(true).sendUpdateNow();
    }
}

void nodeMusicBoxInit(void)
{
    MusicBox.init();
    sensorTimer.initializeMs(200, sensorTask).start();
}
