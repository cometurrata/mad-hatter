#include "node.h"

Node &Node::addNodeType(NodeTypeEnum type)
{
	types.add(type);
	return *this;
}

Node &Node::setHostname(String target)
{
	hostname = target;
	return *this;
}

Node &Node::Register(String target)
{
	JsonObjectStream * data = buildJsonStream();
	sendRegisterRequest(data, NULL);
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
	for (int i = 0 ; i < this->types.size(); i++)
	{
		types.add(this->types[i]);
	}


	return stream;
}

