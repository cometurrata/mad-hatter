#pragma once

class FiguresPlotter
{
private:
    static const uint8_t password[4] = {7, 3, 8, 3};
    uint8_t paswordIdx = 0;

    Timer showPasswordTimer;
    Timer initTimer;
    int figure = 0;

    void task();
    void showPasswordTask();

public:
    void init(void);
    void startShowingPassword(void);
};

