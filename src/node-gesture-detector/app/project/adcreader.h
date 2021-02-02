#pragma once
#include <SmingCore.h>

class AdcReader
{
    uint16_t last_value_ = 0;

public:
    uint16_t read();
    uint16_t getLastVal();
};

class Debouncer
{
    /*
    * Debouncer introduces a delay on value toggling so it cannot toggle faster than 50ms
    */
private:
    Timer timeoutTimer_;
    time_t timeoutDuration_ = 70;
    bool shouldIgnoreValue_ = false;

    void timeoutCallback_();

public:
    void setTimeout(time_t timeout);
    void toggling();
    bool shouldIgnoreValue();
};

class AdcReaderHysteresisPoller
{
    /* 
    * Fetches ADC data every period (default=20ms)
    */

private:
    AdcReader adc;
    Debouncer debouncer_;
    Timer pollTimer;
    time_t period_ = 20;
    time_t debouncerDelay_ = 50;

    uint16_t hysteresisLowLevel = 50;
    uint16_t hysteresisHighLevel = 200;

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
