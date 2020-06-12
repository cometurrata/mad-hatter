#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "ledcontroller.h"

static Timer nodeTimer;
static int figure = 0;
void task(void)
{

    LedController::instance()->lightUpFigure(figure);
    Serial.printf("Figure %d\n", figure);
    figure++;
    if (figure >= 10)
    {
        figure = 0;
    }
}

void nodeFigurePlotterInit(void)
{
    LedController::instance()->init();
    nodeTimer.initializeMs(1000, task).start();
}
