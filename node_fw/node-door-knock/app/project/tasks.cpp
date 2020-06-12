#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "tasks.h"


#define ACTUATE_DUR  60 * 1000
#define ACTUATE_COUNT 1
#define ON LOW
#define OFF HIGH

static Timer heartBeatTimer;
static Timer actuateTimer;

static int pinStatus = OFF;
static int actuateCount = 0;

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(HEARTBEAT_TIME_MS, sendHeartBeat).start();
}

void knockActuate() {
	actuateCount = 0;
	turnOn();
	actuateCount ++;
	actuateTimer.initializeMs(ACTUATE_DUR, actuateCallback).start();
}

int turnOn() {
	debugf("turnOn\n");
	pinStatus = ON;
	digitalWrite(0, pinStatus);
	return pinStatus;
}

int turnOff() {
	debugf("turnOff\n");
	pinStatus = OFF;
	digitalWrite(0, pinStatus);
	return pinStatus;
}

void actuateCallback() {
	debugf("actuateCallback\n");

	if (actuateCount >= ACTUATE_COUNT*2) {
		turnOff();
		actuateTimer.stop();
		return;
	}
	debugf("pinStatus: %d", pinStatus);

	if (pinStatus == HIGH) {
		turnOn();
	}
	else {
		turnOff();
	}
	actuateCount++;
}