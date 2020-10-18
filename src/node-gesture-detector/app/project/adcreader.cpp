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

void AdcReaderHysteresisPoller::start()
{
    pollTimer.initializeMs(period, std::bind(&AdcReaderHysteresisPoller::readAdc, this));
}

void AdcReaderHysteresisPoller::readAdc()
{
    adc.read();

    if (adc.getLastVal() <= hysteresisLowLevel)
    {
        if (lastDigital_ != LOW &&
            onLowUserCb != nullptr)
        {
            onLowUserCb();
        }
        lastDigital_ = LOW;
    }

    if (adc.getLastVal() >= hysteresisHighLevel)
    {
        if (lastDigital_ != HIGH &&
            onHighUserCb != nullptr)
        {
            onHighUserCb();
        }
        lastDigital_ = HIGH;
    }
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
