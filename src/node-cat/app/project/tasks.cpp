#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "tasks.h"

#define ACTUATE_DUR 200
#define ACTUATE_COUNT 3
#define ON HIGH
#define OFF LOW

Node NodeCat;
static Timer actuateTimer;

static int pinStatus = OFF;
static int actuateCount = 0;


void nodeCatInit()
{
	pinMode(2, OUTPUT);
    pinMode(5, OUTPUT);
    digitalWrite(2, HIGH);
    digitalWrite(5, OFF);

}

void catActuate() {
	debugf("startTimer\n");
	turnOn();
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
