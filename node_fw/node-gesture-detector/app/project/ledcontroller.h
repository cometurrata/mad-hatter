#pragma once

// TODO: Configure proper gpio pins
#define LED_RIGHT 4
#define LED_LEFT  5
#define LED_UP    12
#define LED_DOWN  14
#define NB_LEDS   4

class LedController
{
private:
    int currentLedOnId = -1;
    const uint8_t validLedIdsList[NB_LEDS] = {LED_RIGHT, LED_LEFT, LED_UP, LED_DOWN};

    int isValidLedId(int ledId)
    {
        int i;
        for (i=0; i<NB_LEDS; i++) {
            if (validLedIdsList[i] == ledId) {
                return 1;
            }
        }
        return 0;
    }

    void gpioOpenDrainWrite(byte pin, bool gpioValue)
    {
        if(gpioValue) {
            // "HIGH" state -> configure as High Impedance
            pinMode(pin, INPUT);
        }
        else {
            // "LOW" state -> Configure as output connected to GND
            pinMode(pin, OUTPUT);
            digitalWrite(pin, LOW);
        }
    }

public:
    void init()
    {
        int i;
        for (i=0; i<NB_LEDS; i++) {
            turnOff(i);
        }
    }

    void turnOff(int ledId)
    {
        if (isValidLedId(ledId)) {
            gpioOpenDrainWrite(ledId, HIGH);
            currentLedOnId = -1;
        }
    }

    void turnOff()
    {
        turnOff(currentLedOnId);
    }

    void turnOn(int ledId)
    {
        if (isValidLedId(ledId)) {
            gpioOpenDrainWrite(ledId, LOW);
            currentLedOnId = ledId;
        }
    }

    void turnOnAll()
    {
        int i;
        for (i=0; i<NB_LEDS; i++) {
            turnOn(validLedIdsList[i]);
        }
    }

    void turnOffAll()
    {
        int i;
        for (i=0; i<NB_LEDS; i++) {
            turnOff(validLedIdsList[i]);
        }
    }
};