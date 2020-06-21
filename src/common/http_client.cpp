#include "http_client.h"
#include <stdint.h>
#include "wifi.h"

ServerCommunicator &ServerCommunicator::setOnHttpRequestResponseCallback(RequestCompletedDelegate onHttpRequestResponse)
{
    onHttpRequestResponsecb = onHttpRequestResponse;
    return *this;
}

ServerCommunicator &ServerCommunicator::sendNodeUpdate(ReadWriteStream *data)
{
    if (!Wifi.ensureConnected())
    {
        debugf("Not connected to Wifi. Not sending httprequest");
    }
    else
    {
        debugf("sendNodeUpdate");
        HttpRequest *postRequest = new HttpRequest(URL(getServerAddress()));

        postRequest->setMethod(HTTP_POST)
            ->setHeaders(getHttpHeaders())
            ->setBody(data);

        this->sendHttpRequest(postRequest);
    }

    return *this;
}

ServerCommunicator &ServerCommunicator::sendHttpRequest(HttpRequest *req)
{
    if (onHttpRequestResponsecb != nullptr)
    {
        req->onRequestComplete(onHttpRequestResponsecb);
    }
    else
    {
        req->onRequestComplete(RequestCompletedDelegate(&ServerCommunicator::defaultRequestCompleteCb, this));
    }

    bool success = httpClient.send(req);
    if (success)
    {
        debugf("Successfully sent request\n");
    }
    else
    {
        debugf("Failed to send request\n");
    }
    return *this;
}

int ServerCommunicator::defaultRequestCompleteCb(HttpConnection &connection, bool success)
{
    /*
    debugf("\n=========[ URL: %s ]============", connection.getRequest()->uri.toString().c_str());
    debugf("Got response code: %d", connection.getResponse()->code);
    debugf("Success: %d", success);
    */
    if (connection.getRequest()->method != HTTP_HEAD)
    {
        debugf("Got content starting with: %s", connection.getResponse()->getBody().substring(0, 1000).c_str());
    }

    return 0; // return 0 on success in your callbacks
}

String ServerCommunicator::getServerAddress()
{
    String serverAddress = "http://" SERVER_IP ":" SERVER_PORT "/";
    return serverAddress;
}

HttpHeaders ServerCommunicator::getHttpHeaders()
{
    HttpHeaders headers;
    headers["User-Agent"] = "HttpClient/Sming"; // Prefer use of enumerated type for standard field names
    headers["X-Powered-By"] = "Sming";          // Use text for non-standard field names
    headers["Content-Type"] = "application/json";
    return headers;
}
