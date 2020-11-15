#pragma once
#include <SmingCore.h>

class AdcReader
{
    uint16_t last_value_ = 0;

public:
    uint16_t read();
    uint16_t getLastVal();
};

class AdcReaderHysteresisPoller
{
    AdcReader adc;
    Timer pollTimer;
    time_t period_ = 20;

    const uint16_t hysteresisLowLevel = 200;
    const uint16_t hysteresisHighLevel = 800;

    Delegate<void()> onHighUserCb = nullptr;
    Delegate<void()> onLowUserCb = nullptr;

    void readAdc();

    uint16_t lastDigital_;

public:
    void start(time_t period = 20);
    void setHysteresisHighLevel(uint16_t highLevel = 800);
    void setHysteresisLowLevel(uint16_t lowLevel = 200);
    uint16_t getDigital();
    void setOnHighCallBack(Delegate<void()> userCb);
    void setOnLowCallBack(Delegate<void()> userCb);
};
