#include <string.h>

#include <SmingCore.h>

#include "wifi.h"
#include "http_server.h"
#include "node_register.h"
//  --------- EXAMPLE CODE -----------
#include "project/gesturesensor.h"
#include "project/tasks.h"
#include "project/switch.h"

static LedController ledController;

static void ShowInfo()
{
    Serial.printf("\r\nSDK: v%s\r\n", system_get_sdk_version());
    Serial.printf("Free Heap: %d\r\n", system_get_free_heap_size());
    Serial.printf("CPU Frequency: %d MHz\r\n", system_get_cpu_freq());
    Serial.printf("System Chip ID: 0x%x\r\n", system_get_chip_id());
    Serial.printf("SPI Flash ID: 0x%x\r\n", spi_flash_get_id());
    Serial.printf("SPI Flash Size: %d\r\n", (1 << ((spi_flash_get_id() >> 16) & 0xff)));
    //update_print_config();
}

Timer restartTimer;

void onActuate(HttpRequest &request, HttpResponse &response)
{
    restartTimer.initializeMs(1000, TimerDelegate([] {
                                  System.restart();
                                  WDT.alive();
                                  delay(2000);
                              }))
        .startOnce();
    response.sendString("OK");
}

void onWifiOk()
{
    Serial.print(_F("I'm CONNECTED to "));
    debugf("AP. ip: %s mac: %s hostname: %s", WifiStation.getIP().toString().c_str(), WifiStation.getMAC().c_str(), WifiStation.getHostname().c_str());
    serverAddRoute("/actuate", onActuate);
    startWebServer();
}

// Will be called when WiFi hardware and software initialization was finished
// And system initialization was completed
static void ready()
{
    debugf("READY!");

    ShowInfo();

    // Init wifi
    Wifi.setSSID(WIFI_SSID);
    Wifi.setPassword(WIFI_PASSWORD);
    Wifi.startConnect();
    Wifi.setOnConnectUserCb(onWifiOk);

    nodeGestureDetector.addNodeType(Node::NodeTypeEnum::SENSOR_)
        .addNodeType(Node::NodeTypeEnum::ACTUATOR_)
        .setHostname(NODE_HOSTNAME)
        .start();

    ledController.init();
    GestureSensor.init(&ledController);
}

void init()
{
    Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
    Serial.systemDebugOutput(true); // Enable debug output to serial
    Serial.println("Sming. Let's do smart things!");

    // Set system ready callback method
    System.onReady(ready);
}