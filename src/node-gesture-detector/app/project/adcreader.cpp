#include "adcreader.h"

uint16_t AdcReader::read()
{
    static const int number_of_sample_to_read = 1;
    uint16_t adc_value = 0;
    uint8_t adc_clk_div = 8;

    system_adc_read_fast(&adc_value, number_of_sample_to_read, adc_clk_div);
    last_value_ = adc_value;
    return adc_value;
}

uint16_t AdcReader::getLastVal()
{
    return last_value_;
}

void AdcReaderHysteresisPoller::start(time_t period /* = 20 */)
{
    this->period_ = period;
    pollTimer.initializeMs(period_, std::bind(&AdcReaderHysteresisPoller::readAdc, this)).start();
}

void AdcReaderHysteresisPoller::readAdc()
{
    uint16_t adcVal = adc.read();

    if (debouncer_.shouldIgnoreValue())
    {
        debugf("Not processing adc, debouncing in progress...");
        return;
    }
    if (adcVal <= hysteresisLowLevel)
    {
        if (lastDigital_ != LOW &&
            onLowUserCb != nullptr)
        {
            onLowUserCb();
            lastDigital_ = LOW;
            debouncer_.toggling();
        }
    }
    else if (adcVal >= hysteresisHighLevel)
    {
        if (lastDigital_ != HIGH &&
            onHighUserCb != nullptr)
        {
            onHighUserCb();
            lastDigital_ = HIGH;
            debouncer_.toggling();
        }
    }
}

void Debouncer::toggling()
{
    shouldIgnoreValue_ = true;
    timeoutTimer_.initializeMs(timeoutDuration_, std::bind(&Debouncer::timeoutCallback_, this))
        .startOnce();
}

void Debouncer::timeoutCallback_()
{
    shouldIgnoreValue_ = false;
}

void Debouncer::setTimeout(time_t timeout)
{
    timeoutDuration_ = timeout;
}

bool Debouncer::shouldIgnoreValue()
{
    return shouldIgnoreValue_;
}

void AdcReaderHysteresisPoller::setHysteresisHighLevel(uint16_t highLevel)
{
    hysteresisHighLevel = highLevel;
}

void AdcReaderHysteresisPoller::setHysteresisLowLevel(uint16_t lowLevel)
{
    hysteresisLowLevel = lowLevel;
}

uint16_t AdcReaderHysteresisPoller::getDigital()
{
    return lastDigital_;
}

void AdcReaderHysteresisPoller::setOnHighCallBack(Delegate<void()> userCb)
{
    onHighUserCb = userCb;
}

void AdcReaderHysteresisPoller::setOnLowCallBack(Delegate<void()> userCb)
{
    onLowUserCb = userCb;
}
