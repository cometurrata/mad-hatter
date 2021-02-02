#include "lightswitch.h"

#define ON HIGH
#define OFF LOW

SwitchClass Switch;

void SwitchClass::onAdcToggleCb_()
{
    if (adcPoller_.getDigital() == ON)
    {
        debugf("Switch detected ON");
        sensorNode_->setIsOn(true);
    }
    else if (adcPoller_.getDigital() == OFF)
    {
        debugf("Switch detected OFF");
        sensorNode_->setIsOn(false);
    }
    sensorNode_->sendUpdateNow();
}

void SwitchClass::init(Node *sensorNode)
{
    sensorNode_ = sensorNode;

    adcPoller_.setOnHighCallBack(std::bind(&SwitchClass::onAdcToggleCb_, this));
    adcPoller_.setOnLowCallBack(std::bind(&SwitchClass::onAdcToggleCb_, this));
    adcPoller_.start();
}
