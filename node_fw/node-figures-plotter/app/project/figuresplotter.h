#pragma once

class FiguresPlotter
{
private:
    static const uint8_t password[4];
    uint8_t passwordIdx = 0;

    Timer showPasswordTimer;
    Timer initTimer;
    int figure = 0;

    void task();
    void showPasswordTask();

public:
    void init(void);
    void startShowingPassword(void);
};

