#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "tasks.h"

#define ACTUATE_DUR 200
#define ACTUATE_COUNT 3
#define ON HIGH
#define OFF LOW

static Timer heartBeatTimer;
static Timer actuateTimer;

static int pinStatus = OFF;
static int actuateCount = 0;

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(HEARTBEAT_TIME_MS, sendHeartBeat).start();
}

void nodeCatInit()
{
	pinMode(2, OUTPUT);
    pinMode(5, OUTPUT);
    digitalWrite(2, HIGH);
    digitalWrite(5, OFF);

    actuateTimer.initializeMs(ACTUATE_DUR, actuateCallback);
}

void catActuate() {
	debugf("startTimer\n");
	actuateCount = 0;
	turnOn();
	actuateCount ++;
	actuateTimer.start();
}

int turnOn() {
	debugf("turnOn\n");
	pinStatus = ON;
	digitalWrite(2, pinStatus);
	digitalWrite(5, pinStatus);
	return pinStatus;
}

int turnOff() {
	debugf("turnOff\n");
	pinStatus = OFF;
	digitalWrite(2, pinStatus);
	digitalWrite(5, pinStatus);
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
