#include <SmingCore.h>

// #include "LSM9DS1.h"
#include "MPU6050.h"
#define I2C_AXL_GYR_ADDRESS (0x6b)
#define I2C_MAGNETOMETER_ADDRESS (0x1e)

#define SDA_PIN 4
#define SCL_PIN 5

Timer procTimer;

uint8_t i = 0;
uint8_t data[10];
// LSM9DS1 imu;

MPU6050 mpu;

bool IRAM_ATTR doesPatternMatch(uint8_t *targetPattern, uint8_t patternLength,
                                uint32_t *receivedPattern, uint8_t tolerancePercentage);

void knockDetectionSetup()
{
    Wire.pins(4, 5); // SDA, SCL

    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // I2C frequency at 400 kHz

    if (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
    {
        Serial.println("Failed to communicate with IMU.");
    }
}

int prevx, prevy, prevz;

uint32_t filteredValue = 0;
uint32_t presentWeight = 4;
uint32_t pastWeight = 7;
uint32_t thresholdHigh = 4000;
uint32_t thresholdLow = 400;

bool isKnockDetected = false;

void pushEvent(bool knocking);
void pushAKnock(uint32_t startTime, uint32_t endTime);

uint32_t IRAM_ATTR distance(int a, int b)
{
    int32_t diff = (int32_t)a - (int32_t)b;
    if (diff < 0)
    {
        diff = -diff;
    }
    return diff;
}

uint32_t stopKnocking = 0;

void IRAM_ATTR loop()
{
    int16_t x, y, z;
    Arduino_Vector v = mpu.readRawAccel();
    x = v.XAxis;
    y = v.YAxis;
    z = v.ZAxis;

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
        debugf("Knock");
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
    // if (filteredValue > 3)
    // {
    //     // debugf("val : %d", filteredValue);
    // }
    // debugf("x : %d", x);
}

bool knockingProcessed = false;
uint32_t knockingDuration = 0;
uint32_t knockingStartTime = 0;
uint32_t knockingEndTime = 0;

void IRAM_ATTR pushEvent(bool knocking)
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

uint8_t knockPattern[10] = {20, 10, 10, 10, 10, 10, 10};
uint8_t knockPatternLength = 7;
uint8_t knockIndex = 0;
uint32_t knockArray[7];

bool knockingMatched = false;

uint32_t tmpStartTime = 0;

uint16_t c = 0;
void IRAM_ATTR pushAKnock(uint32_t startTime, uint32_t endTime)
{
    debugf("Push a Knock %d", c++);
    if (tmpStartTime == 0)
    {
        tmpStartTime = startTime;
        return;
    }
    else
    {
        if (doesPatternMatch(knockPattern, knockPatternLength - 2, knockArray, 20))
        {
            debugf("Success");
            isKnockDetected = true;
        }
        else
        {
            for (int i = 0; i < knockPatternLength - 2; i++)
            {
                knockArray[i] = knockArray[i + 1];
            }
            knockArray[knockPatternLength - 2] = startTime - tmpStartTime;
            tmpStartTime = startTime;

            Serial.println("");

            for (int i = 0; i < knockPatternLength - 2; i++)
            {
                Serial.print(knockArray[i]);
                Serial.print(", ");
            }
            Serial.println("");
            for (int i = 0; i < knockPatternLength - 2; i++)
            {
                uint32_t unit = knockArray[0] / knockPattern[0];
                Serial.print(knockPattern[i] * unit);
                Serial.print(", ");
            }
            Serial.println("");
        }
        if (doesPatternMatch(knockPattern, knockPatternLength - 2, knockArray, 20))
        {
            debugf("Success");
            isKnockDetected = true;
        }
    }
}

uint32_t IRAM_ATTR applyPercentage(uint32_t val, int8_t percent)
{
    return ((val * (100 + percent)) / 100);
}

bool IRAM_ATTR doesPatternMatch(uint8_t *targetPattern, uint8_t patternLength,
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

uint32_t IRAM_ATTR filter(uint32_t cumAbsDiff, uint16_t oldWeight, uint16_t newAbsDiff)
{
    return cumAbsDiff + newAbsDiff;
}

void IRAM_ATTR knockDetectionInit()
{
    knockDetectionSetup();
    for (int i = 0; i < 6; i++)
    {
        knockArray[i] = 200;
    }

    Arduino_Vector a = mpu.readRawAccel();

    prevx = a.XAxis;
    prevy = a.YAxis;
    prevz = a.ZAxis;

    procTimer.initializeMs(3, loop).start();
    debugf("Knock detection initilised");
}

bool getKnockDetected()
{
    return isKnockDetected;
}
