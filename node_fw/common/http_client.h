#pragma once

#include <SmingCore.h>

class ServerCommunicator
{
private:
    HttpClient httpClient;
    RequestCompletedDelegate onHttpRequestResponsecb = nullptr;
    int defaultRequestCompleteCb(HttpConnection &connection, bool success);

    String getServerAddress();
    HttpHeaders getHttpHeaders();
    ServerCommunicator &sendHttpRequest(HttpRequest *req);

public:
    ServerCommunicator &setOnHttpRequestResponseCallback(RequestCompletedDelegate onHttpRequestResponse);
    ServerCommunicator &sendNodeUpdate(ReadWriteStream *data);
};
