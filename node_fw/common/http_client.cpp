#include <stdint.h>
#include <SmingCore.h>

static HttpClient httpClient;

void sendHttpRequest(String path, String data, RequestCompletedDelegate onHttpRequestResponse)
{
    String serverAddress = "http://" SERVER_IP ":" SERVER_PORT "/";
    HttpRequest *postRequest = new HttpRequest(URL(serverAddress + path));

    debugf("Sending request to : %s", serverAddress.c_str());
    debugf("path : %s", path.c_str());
    debugf("data : %s", data.c_str());

    HttpHeaders headers;
    headers["User-Agent"] = "HttpClient/Sming"; // Prefer use of enumerated type for standard field names
    headers["X-Powered-By"] = "Sming";          // Use text for non-standard field names
    headers["Content-Type"] = "application/json";

    postRequest->setMethod(HTTP_POST)
        ->setHeaders(headers)
        ->setBody(data)
        ->onRequestComplete(onHttpRequestResponse);

    bool success = httpClient.send(postRequest);
    if (success)
    {
        debugf("Successfully sent request\n");
    }
    else
    {
        debugf("Failed to send request\n");
    }
}

static int onNodeUpdateRequestResponse(HttpConnection &connection, bool success)
{
    debugf("\n=========[ URL: %s ]============", connection.getRequest()->uri.toString().c_str());
    debugf("Got response code: %d", connection.getResponse()->code);
    debugf("Success: %d", success);
    if (connection.getRequest()->method != HTTP_HEAD)
    {
        debugf("Got content starting with: %s", connection.getResponse()->getBody().substring(0, 1000).c_str());
    }

    return 0; // return 0 on success in your callbacks
}

void sendNodeUpdate(String data)
{
    sendHttpRequest(NODE_HOSTNAME, data, onNodeUpdateRequestResponse);
}

void sendHeartBeat()
{
    sendHttpRequest(String(NODE_HOSTNAME) + "/alive", "heartbeat", onNodeUpdateRequestResponse);
}
