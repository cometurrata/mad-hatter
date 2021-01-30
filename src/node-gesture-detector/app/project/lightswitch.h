#pragma once

#include <SmingCore.h>
#include "node.h"
#include "adcreader.h"

class SwitchClass
{
    friend Timer;

private:
    Node *sensorNode_ = nullptr;
    AdcReaderHysteresisPoller adcPoller_;

private:
    void onAdcToggleCb_();

public:
    void init(Node *sensorNode);
};

extern SwitchClass Switch;
