#include "node_gesture_sensor.h"

static NodeGestureSensor node;
static LedController ledControllerInstance;


NodeGestureSensor &NodeGestureSensor::instance()
{
    return node;
}

void NodeGestureSensor::init(void)
{
    ledControllerInstance.init();
    GestureSensor.init(&ledControllerInstance);
}
