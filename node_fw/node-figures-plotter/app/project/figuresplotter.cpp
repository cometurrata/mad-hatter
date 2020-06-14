#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "ledcontroller.h"
#include "figuresplotter.h"

const uint8_t FiguresPlotter::password[4] = {7, 3, 8, 3};

void FiguresPlotter::task(void)
{
    LedController::instance()->lightUpFigure(figure);
    Serial.printf("Figure %d\n", figure);
    figure++;
    if (figure >= 10)
    {
        initTimer.stop();
    }
}

void FiguresPlotter::init(void)
{
    LedController::instance()->init();
    LedController::instance()->setLightUpDuration(1000);
    initTimer.initializeMs(250, std::bind(&FiguresPlotter::task, this)).start();
    figure = 0;
}

void FiguresPlotter::showPasswordTask()
{
    LedController::instance()->lightUpFigure(password[passwordIdx]);
    passwordIdx++;
    if (passwordIdx < sizeof(FiguresPlotter::password))
    {
        showPasswordTimer.startOnce();
    }
}

void FiguresPlotter::startShowingPassword(void)
{
    passwordIdx = 0;
    initTimer.stop();
    showPasswordTimer.initializeMs(1000, std::bind(&FiguresPlotter::showPasswordTask, this)).start();
}

