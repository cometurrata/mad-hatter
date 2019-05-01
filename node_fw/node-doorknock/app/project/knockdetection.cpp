#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "SparkFun_ADXL345.h"

#define SDA_PIN 4
#define SCL_PIN 5

Timer procTimer;

uint8_t i = 0;
uint8_t data[10];
ADXL345 adxl = ADXL345();

bool doesPatternMatch(uint8_t *targetPattern, uint8_t patternLength,
                      uint32_t *receivedPattern, uint8_t tolerancePercentage);

void knockDetectionSetup()
{
    Wire.pins(SDA_PIN, SCL_PIN);
    adxl.powerOn();
    adxl.setRangeSetting(2);
}

int prevx, prevy, prevz;

uint32_t filteredValue = 0;
uint32_t presentWeight = 4;
uint32_t pastWeight = 7;
uint32_t thresholdHigh = 17;
uint32_t thresholdLow = 3;

bool isKnockDetected = false;

void pushEvent(bool knocking);
void pushAKnock(uint32_t startTime, uint32_t endTime);

uint32_t distance(int a, int b)
{
    int32_t diff = (int32_t)a - (int32_t)b;
    if (diff < 0)
    {
        diff = -diff;
    }
    return diff;
}

uint32_t stopKnocking = 0;

void loop()
{
    int x, y, z;
    adxl.readAccel(&x, &y, &z);

    static bool isKnocking = false;

    filteredValue = (pastWeight * filteredValue) +
                    (presentWeight * (distance(x, prevx) +
                                      distance(y, prevy) +
                                      distance(z, prevz)));
    filteredValue /= (pastWeight + presentWeight);

    if (isKnocking == false &&
        filteredValue > thresholdHigh && (millis() - stopKnocking > 50))
    {
        isKnocking = true;
        // debugf("Knock");
    }
    else if (isKnocking && filteredValue < thresholdLow)
    {
        // debugf(".");
        isKnocking = false;
        stopKnocking = millis();
    }

    pushEvent(isKnocking);

    prevx = x;
    prevy = y;
    prevz = z;
    if (filteredValue > 3)
    {
        // debugf("val : %d", filteredValue);
    }
}

bool knockingProcessed = false;
uint32_t knockingDuration = 0;
uint32_t knockingStartTime = 0;
uint32_t knockingEndTime = 0;

void pushEvent(bool knocking)
{
    if (knocking && !knockingProcessed)
    {
        knockingDuration = 0;
        knockingProcessed = true;
        knockingStartTime = millis();
    }
    else if (!knocking && knockingProcessed)
    {
        knockingProcessed = false;
        knockingEndTime = millis();
        pushAKnock(knockingStartTime, knockingEndTime);
    }
}

uint8_t knockPattern[10] = {20, 10, 10, 20, 40, 20, 20};
uint8_t knockPatternLength = 7;
uint8_t knockIndex = 0;
uint32_t knockArray[7];

bool knockingMatched = false;

uint32_t tmpStartTime = 0;

uint16_t c = 0;
void pushAKnock(uint32_t startTime, uint32_t endTime)
{
    debugf("Push a Knock %d", c++);
    if (tmpStartTime == 0)
    {
        tmpStartTime = startTime;
        return;
    }
    else
    {
        if (doesPatternMatch(knockPattern, 5, knockArray, 20))
        {
            debugf("Success");
            isKnockDetected = true;
        }
        else
        {
            for (int i = 0; i < 5; i++)
            {
                knockArray[i] = knockArray[i + 1];
            }
            knockArray[5] = startTime - tmpStartTime;
            tmpStartTime = startTime;

            Serial.println("");

            for (int i = 0; i < 5; i++)
            {
                Serial.print(knockArray[i]);
                Serial.print(", ");
            }
            Serial.println("");
            for (int i = 0; i < 5; i++)
            {
                uint32_t unit =  knockArray[0] / knockPattern[0] ;
                Serial.print(knockPattern[i] * unit);
                Serial.print(", ");
            }
            Serial.println("");
        }
        if (doesPatternMatch(knockPattern, 5, knockArray, 20))
        {
            debugf("Success");
        }
    }
}

uint32_t applyPercentage(uint32_t val, int8_t percent)
{
    return ((val * (100 + percent)) / 100);
}

bool doesPatternMatch(uint8_t *targetPattern, uint8_t patternLength,
                      uint32_t *receivedPattern, uint8_t tolerancePercentage)
{
    uint32_t unit = receivedPattern[0] / targetPattern[0];
    if (patternLength < 2)
    {
        return true;
    }

    for (int i = 0; i < patternLength; i++)
    {
        if (targetPattern[i] * unit < receivedPattern[i])
        {

            if (applyPercentage(targetPattern[i] * unit, tolerancePercentage) < receivedPattern[i])
            {
                return false;
            }
        }
        if (targetPattern[i] * unit > receivedPattern[i])
        {
            if (applyPercentage(targetPattern[i] * unit, -tolerancePercentage) > receivedPattern[i])
            {
                return false;
            }
        }
    }
    return true;
}

uint32_t filter(uint32_t cumAbsDiff, uint16_t oldWeight, uint16_t newAbsDiff)
{
    return cumAbsDiff + newAbsDiff;
}

void knockDetectionInit()
{
    knockDetectionSetup();
    for (int i = 0; i < 6; i++)
    {
        knockArray[i] = 200;
    }
    adxl.readAccel(&prevx, &prevy, &prevz);
    procTimer.initializeMs(3, loop).start();
}

bool getKnockDetected()
{
    return isKnockDetected;
}

