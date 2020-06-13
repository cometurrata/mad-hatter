#include "node.h"

Node &Node::addNodeType(NodeTypeEnum type)
{
	types.add(type);
	return *this;
}

Node &Node::start();
{
    updateTimer.initializeMs(HEARTBEAT_TIME_MS,std::bind(&Node::update, this)).start();
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

Node &Node::update()
{
	JsonObjectStream *data = buildJsonStream();
	serverCommunicator.sendNodeUpdate(data);
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
		types.add(this->types[i]);
	}

	return stream;
}
