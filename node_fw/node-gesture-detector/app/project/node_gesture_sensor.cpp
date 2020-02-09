#include "node_gesture_sensor.h"

static NodeGestureSensor node;
static LedController ledControllerInstance;
static CharliePlexing charlieplexingInstance;

void NodeGestureSensor::nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(30 * 1000, sendHeartBeat);
}

NodeGestureSensor &NodeGestureSensor::instance()
{
    return node;
}

void NodeGestureSensor::init(void)
{
    nodeHeartBeatInit();
    ledControllerInstance.init(&charlieplexingInstance);
    GestureSensor.init(&ledControllerInstance);
}
