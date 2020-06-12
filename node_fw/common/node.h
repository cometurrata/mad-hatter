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

	Node &addNodeType(Node::NodeTypeEnum type);
	Node &setHostname(String target);
	Node &Register(String target);


private:
	Vector<enum Node::NodeTypeEnum> types;
	String hostname;
	bool solved;

	JsonObjectStream *buildJsonStream();

};