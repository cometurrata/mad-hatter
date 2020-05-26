#pragma once

#define LED_RIGHT 12
#define LED_LEFT  13
#define LED_UP    14
#define LED_DOWN  16
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
            /**
             * If we don't reconfigure the gpio direction to be HIGH,
             * there is a small current leak that can keep the LEDs slightly turned on
             **/
            digitalWrite(pin, HIGH);
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