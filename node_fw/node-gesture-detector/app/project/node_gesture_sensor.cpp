#include "node_gesture_sensor.h"

static NodeGestureSensor node = NodeGestureSensor();

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
    GestureSensor.init(&(CharliePlexing::instance()));
}
