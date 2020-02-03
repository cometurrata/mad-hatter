#pragma once 
#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "gesturesensor.h"
#include "charlieplexing.h"

class NodeGestureSensor
{
private:
    Timer sensorTimer;
    Timer heartBeatTimer;
    void nodeHeartBeatInit();

public:
    static NodeGestureSensor &instance();
    void init(void);
};
