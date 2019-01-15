#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "common/http_client.h"

static HttpServer server;

static void onIndex(HttpRequest& request, HttpResponse& response)
{
	debugf("On INDEX\n");
	response.code = HTTP_STATUS_OK;
        response.sendString("<html><body><h1>Cool!</h1></body></html>");
}

static void onDefault(HttpRequest& request, HttpResponse& response)
{
	debugf("Unknown path requested: %s\n", request.getPath().c_str());
	response.notFound();
}

void startWebServer(void)
{
	debugf("==== STARTING WEB SERVER ====\n");
	server.listen(80);
	server.addPath("/", onIndex);
	server.setDefaultHandler(onDefault);
}