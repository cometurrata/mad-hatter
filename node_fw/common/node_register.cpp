#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "http_client.h"
#include "node_register.h"

static Timer retryTimer;

static int onRegisterNodeRequestResponse(HttpConnection &connection, bool success)
{
    if (!success)
    {
        debugf("HTTP Register FAILURE\n");
        retryTimer.initializeMs(10000, registerNode).startOnce();
    }
    else
    {
        debugf("HTTP Register SUCCESS\n");
    }

    return 0;
}

void registerNode(void)
{
    String register_data = "{"
                           "\"ip\":\"" +
                           WifiStation.getIP().toString() +
                           "\",\"hostname\":\"" NODE_HOSTNAME "\""
                           "}";

    sendHttpRequest("register", register_data, onRegisterNodeRequestResponse);
}