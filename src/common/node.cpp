#include "node.h"

Node &Node::addNodeType(NodeTypeEnum type)
{
	types.add(type);
	return *this;
}

Node &Node::start()
{
    heartbeatTimer.initializeMs(HEARTBEAT_TIME_MS,std::bind(&Node::sendUpdateNow, this)).start();
	return *this;
}

Node &Node::setHostname(String target)
{
	hostname = target;
	return *this;
}

Node &Node::setSolved(bool target)
{
	this->solved = true;
	return *this;
}

Node &Node::sendUpdateNow()
{
	static time_t lastCall = 0;
	if (millis() - lastCall > 500)
	{
		lastCall = millis();
		JsonObjectStream *data = buildJsonStream();
		serverCommunicator.sendNodeUpdate(data);
	}
	return *this;
}

JsonObjectStream *Node::buildJsonStream()
{
	JsonObjectStream *stream = new JsonObjectStream();
	JsonObject &json = stream->getRoot();

	json["status"] = solved ? "solved" : "unsolved";
	json["ip"] = WifiStation.getIP().toString();
	json["hostname"] = hostname;

	JsonArray &types = json.createNestedArray("types");
	for (int i = 0; i < this->types.size(); i++)
	{
		switch (this->types[i])
		{
		case Node::NodeTypeEnum::ACTUATOR_:
			types.add("actuator");
			break;
		
		case Node::NodeTypeEnum::SENSOR_:
			types.add("sensor");
			break;
		default:
			break;
		}
	}

	return stream;
}
