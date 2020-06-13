#pragma once

#include <SmingCore.h>
#include <JsonObjectStream.h>
#include "http_client.h"

class Node{
public:

	enum NodeTypeEnum{
		ACTUATOR_=0,
		SENSOR_
	};

	Node &start();
	Node &addNodeType(Node::NodeTypeEnum type);
	Node &setHostname(String target);
	Node &update();
	Node &setSolved(bool target);

private:
	ServerCommunicator serverCommunicator;

	Vector<enum Node::NodeTypeEnum> types;
	String hostname = "";
	bool solved = false;
	Timer updateTimer;
	JsonObjectStream *buildJsonStream();
};