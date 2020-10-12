#include "http_client.h"
#include <MultipartParser.h>
#include <HttpMultipartResource.h>
#include <OtaUpgradeStream.h>
#include <FlashString/Array.hpp>

static HttpServer server;
static Timer trigger_timer;

IMPORT_FSTR_LOCAL(uploadHtml, PROJECT_DIR "/../common/" UPLOAD_HTML);
IMPORT_FSTR_LOCAL(submissionHtml, PROJECT_DIR "/../common/" SUBMISSION_HTML);

static void onVersion(HttpRequest &request, HttpResponse &response)
{
    debugf("Version: %s", VERSION);
    response.setCache(86400, true); // It's important to use cache for better performance.
    response.sendString(VERSION);
}

static void onOta(HttpRequest &request, HttpResponse &response)
{
    auto stream = new FSTR::Stream(submissionHtml);
    response.setCache(86400, true); // It's important to use cache for better performance.
    response.sendDataStream(stream, MIME_HTML);
}

static int onUpgrade(HttpServerConnection &connection, HttpRequest &request, HttpResponse &response)
{
    static bool locked = false;
    if (locked)
    {
        return 0;
    }
    locked = true;
    ReadWriteStream *file = request.files["firmware"];
    OtaUpgradeStream *otaStream = static_cast<OtaUpgradeStream *>(file);
    debugf("onUpgrade !");
    if (otaStream == nullptr)
    {
        debug_e("Something went wrong with the file upload");
        return 1;
    }

    if (response.isSuccess() && !otaStream->hasError())
    {
        // defer the reboot by 1000 milliseconds to give time to the web server to return the response
        System.restart(1000);

        auto stream = new FSTR::Stream(uploadHtml);
        response.headers[HTTP_HEADER_CONNECTION] = "close";
        return response.sendDataStream(stream, MIME_HTML);
    }

    response.code = HTTP_STATUS_BAD_REQUEST;
    response.setContentType(MIME_HTML);
    String html = "<H2 color='#444'>" + OtaUpgradeStream::errorToString(otaStream->errorCode) + "</H2>";
    response.headers[HTTP_HEADER_CONTENT_LENGTH] = html.length();
    response.sendString(html);

    return 0;
}

static void fileUploadMapper(HttpFiles &files)
{
    /*
     * On a normal computer file uploads are usually using
     * temporary space on the hard disk or in memory to store the incoming data.
     *
     * On an embedded device that is a luxury that we can hardly afford.
     * Therefore we should define a `map` that specifies explicitly
     * by which stream each form field will be consumed.
     *
     * If a field is not specified then its content will be discarded.
     */
    files["firmware"] = new OtaUpgradeStream;
}

static void onIndex(HttpRequest &request, HttpResponse &response)
{
    debugf("On INDEX\n");
    response.code = HTTP_STATUS_OK;
    response.sendString("<html><body><h1>Cool!</h1></body></html>");
}

uint8_t trigger_enabled[18] = {false};
uint8_t trigger_sent[18] = {false};
uint8_t trigger_value[18] = {LOW};

void trigger_task()
{
}

void onConfigurePin(HttpRequest &request, HttpResponse &response)
{
    debugf("onConfigurePin\n");

    String body = request.getBody();

    int pin = request.getQueryParameter("pin").toInt();
    int direction = request.getQueryParameter("direction").toInt();

    if (direction == 1)
    {
        pinMode(pin, INPUT);
    }
    else if (direction == 0)
    {
        pinMode(pin, OUTPUT);
    }

    response.code = HTTP_STATUS_OK;
    response.sendString("OK");
}

void onReadPin(HttpRequest &request, HttpResponse &response)
{
    debugf("onReadPin\n");
    int pin = request.getQueryParameter("pin").toInt();
    pinMode(pin, INPUT);
    int value = digitalRead(pin);

    response.code = HTTP_STATUS_OK;
    response.sendString("{"
                        "\"value\":" +
                        String(value) +
                        ",\"pin\":" + String(pin) +
                        "}");
}

void onWritePin(HttpRequest &request, HttpResponse &response)
{
    debugf("onWritePin\n");
    int pin = request.getQueryParameter("pin").toInt();
    int value = request.getQueryParameter("value").toInt();

    pinMode(pin, OUTPUT);
    digitalWrite(pin, value);

    response.code = HTTP_STATUS_OK;
    response.sendString("{"
                        "\"value\":" +
                        String(value) +
                        ",\"pin\":" + String(pin) +
                        "}");
}

void onAddTriggerPin(HttpRequest &request, HttpResponse &response)
{
    debugf("onAddTriggerPin");
    int pin = request.getQueryParameter("pin").toInt();
    int value = request.getQueryParameter("value").toInt();

    if (pin >= 18 || pin < 0)
    {
        debugf("Error Unknown Pin\r\n");
        response.code = HTTP_STATUS_OK;
        response.sendString("{"
                            "\"error\": \"Unknown Pin\""
                            "}");
    }
    else
    {
        debugf("Trigger Added \r\n");
        trigger_enabled[pin] = true;
        trigger_value[pin] = value;
        trigger_sent[pin] = false;
        response.code = HTTP_STATUS_OK;
        response.sendString("{"
                            "\"status\": \"OK\""
                            "}");
    }
}

void onRemoveTriggerPin(HttpRequest &request, HttpResponse &response)
{
    debugf("onRemoveTriggerPin");
    int pin = request.getQueryParameter("pin").toInt();

    if (pin >= 18)
    {
        debugf("Error Unknown Pin\r\n");
        response.code = HTTP_STATUS_OK;
        response.sendString("{"
                            "\"error\": \"Unknown Pin\""
                            "}");
    }
    else
    {
        debugf("Trigger Removed \r\n");
        trigger_enabled[pin] = false;
        response.code = HTTP_STATUS_OK;
        response.sendString("{"
                            "\"status\": \"OK\""
                            "}");
    }
}

static void onReboot(HttpRequest &request, HttpResponse &response)
{
    debugf("Received reboot command");
    response.code = HTTP_STATUS_OK;

    System.restart();
    while(true)
    {
        WDT.alive();
        delay(100);
    }
}

static void onDefault(HttpRequest &request, HttpResponse &response)
{
    debugf("Unknown path requested: %s\n", request.uri.Path.c_str());
    response.code = HTTP_STATUS_NOT_FOUND;
}

void startWebServer(void)
{
    debugf("==== STARTING WEB SERVER ====\n");
    server.listen(80);
    server.paths.set("/", onIndex);
    server.paths.set("/reboot", onReboot);
    server.paths.set("/configure-pin", onConfigurePin);
    server.paths.set("/read-pin", onReadPin);
    server.paths.set("/write-pin", onWritePin);
    server.paths.set("/add-trigger", onAddTriggerPin);
    server.paths.set("/remove-trigger", onRemoveTriggerPin);
    server.paths.set("/ota", onOta);
    server.paths.set("/upgrade", new HttpMultipartResource(fileUploadMapper, onUpgrade));
    server.paths.set("/version", onVersion);
    server.paths.setDefault(onDefault);

    server.setBodyParser(MIME_FORM_MULTIPART, formMultipartParser);

    trigger_timer.initializeMs(100, trigger_task).start();
}

void serverAddRoute(String path, HttpPathDelegate callback)
{
    server.paths.set(path, callback);
}

void httpServerAddPath(String path, const HttpPathDelegate &callback)
{
    server.paths.set(path, callback);
}