#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "tasks.h"

const int NodeLightRoomClass::ON = HIGH;
const int NodeLightRoomClass::OFF = LOW;

NodeLightRoomClass NodeLightRoom;

void NodeLightRoomClass::init()
{
	pinMode(lightGpio_, OUTPUT);
	digitalWrite(lightGpio_, OFF);
}

void NodeLightRoomClass::actuate()
{
	toggle();
}

int NodeLightRoomClass::toggle()
{
	if (pinStatus == ON)
	{
		return turnOff();
	}

	return turnOn();
}

int NodeLightRoomClass::turnOn()
{
	debugf("turnOn\n");
	pinStatus = ON;
	digitalWrite(lightGpio_, pinStatus);
	return pinStatus;
}

int NodeLightRoomClass::turnOff()
{
	debugf("turnOff\n");
	pinStatus = OFF;
	digitalWrite(lightGpio_, pinStatus);
	return pinStatus;
}
