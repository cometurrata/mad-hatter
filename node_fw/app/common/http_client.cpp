#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

static HttpClient httpClient;

void sendHttpRequest(String path, String data, RequestCompletedDelegate onHttpRequestResponse)
{
    HttpRequest *postRequest = new HttpRequest(URL(String("http://" SERVER_IP "/") + path));
    postRequest->setMethod(HTTP_POST);

    // Headers: if you need to set custom headers then you can do something like ...
    HttpHeaders headers;
    headers[HTTP_HEADER_USER_AGENT] = _F("HttpClient/Sming"); // Prefer use of enumerated type for standard field names
    headers[F("X-Powered-By")] = _F("Sming");                 // Use text for non-standard field names
    postRequest->setHeaders(headers);
    postRequest->setHeader(F("X-Powered-By"), F("Sming"));
    postRequest->setBody(data);
    /*
	 * Notice: If we use SSL we need to set the SSL settings only for the first request
	 * 		   and all consecutive requests to the same host:port will try to reuse those settings
	 */

    // If we want to process the response we can do it by setting a onRequestCallback
    postRequest->onRequestComplete(onHttpRequestResponse);

    // Second: We have to send that request using our httpClient
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
    debugf("RemoteIP: %s", (char *)connection.getRemoteIp());
    debugf("Got response code: %d", connection.getResponseCode());
    debugf("Success: %d", success);
    if (connection.getRequest()->method != HTTP_HEAD)
    {
        debugf("Got content starting with: %s", connection.getResponseString().substring(0, 1000).c_str());
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
