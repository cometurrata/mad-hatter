#include "node.h"

Node &Node::addNodeType(NodeTypeEnum type)
{
	types.add(type);
	return *this;
}

Node &Node::start()
{
	debugf("STARTING NODE!");
	if (false == types.contains(NodeTypeEnum::SWITCH_))
	{
		// SWITCH_ nodes !! should not have an HEART_BEAT !!!
		heartbeatTimer.initializeMs(
						  HEARTBEAT_TIME_MS,
						  std::bind(&Node::sendUpdateNow, this))
			.start();
	}
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

Node &Node::setIsOn(bool is_on)
{
	this->is_on = true;
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

		case Node::NodeTypeEnum::SWITCH_:
			types.add("switch");
			json["is_on"] = is_on;
			break;

		default:
			break;
		}
	}

	return stream;
}
