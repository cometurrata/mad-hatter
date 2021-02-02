#pragma once

#include <SmingCore.h>
#include <JsonObjectStream.h>
#include "http_client.h"

class Node
{
public:
	enum NodeTypeEnum
	{
		ACTUATOR_ = 0,
		SENSOR_,
		SWITCH_
	};

	Node &start();
	Node &addNodeType(Node::NodeTypeEnum type);
	Node &setHostname(String target);
	Node &sendUpdateNow();
	Node &setSolved(bool target);
	Node &setIsOn(bool is_on);

private:
	ServerCommunicator serverCommunicator;

	Vector<enum Node::NodeTypeEnum> types;
	String hostname = "";
	bool solved = false;
	bool is_on = false;
	Timer heartbeatTimer;
	JsonObjectStream *buildJsonStream();
};