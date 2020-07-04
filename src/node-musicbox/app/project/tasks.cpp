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
        musicBoxNode.setSolved(true).sendUpdateNow();
    }
}

void nodeMusicBoxInit(void)
{
    MusicBox.init();
    sensorTimer.initializeMs(3 * 1000, sensorTask).start();
}
