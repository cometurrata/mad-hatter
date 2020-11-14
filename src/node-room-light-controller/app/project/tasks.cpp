#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "tasks.h"

#define ON HIGH
#define OFF LOW

Node NodeLightRoom;

static int pinStatus = OFF;
static int toggle();
static int turnOn();
static int turnOff();


void nodeLightRoomInit()
{
	pinMode(2, OUTPUT);
	pinMode(5, OUTPUT);
	digitalWrite(2, HIGH);
	digitalWrite(5, OFF);
}

void lightAcuate()
{
	toggle();
}

static int toggle()
{
	if (pinStatus == ON)
	{
		return turnOff();
	}

	return turnOn();
}

static int turnOn()
{
	debugf("turnOn\n");
	pinStatus = ON;
	digitalWrite(2, pinStatus);
	digitalWrite(5, pinStatus);
	return pinStatus;
}

static int turnOff()
{
	debugf("turnOff\n");
	pinStatus = OFF;
	digitalWrite(2, pinStatus);
	digitalWrite(5, pinStatus);
	return pinStatus;
}
