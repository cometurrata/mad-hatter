#include <string.h>

#include <SmingCore.h>

#include "wifi.h"
#include "http_server.h"
#include "node_register.h"
//  --------- EXAMPLE CODE -----------
#include "project/shroom_task.h"
#include "project/tasks.h"

static void ShowInfo()
{
    debugf("\r\nSDK: v%s\r\n", system_get_sdk_version());
    debugf("Free Heap: %d\r\n", system_get_free_heap_size());
    debugf("CPU Frequency: %d MHz\r\n", system_get_cpu_freq());
    debugf("System Chip ID: 0x%x\r\n", system_get_chip_id());
    debugf("SPI Flash ID: 0x%x\r\n", spi_flash_get_id());
    debugf("SPI Flash Size: %d\r\n", (1 << ((spi_flash_get_id() >> 16) & 0xff)));
    //update_print_config();
}

void wifiOk(IpAddress ip, IpAddress mask, IpAddress gateway)
{
    Serial.print(_F("I'm CONNECTED to "));
    Serial.println(ip);
    debugf("AP. ip: %s mac: %s hostname: %s", WifiStation.getIP().toString().c_str(), WifiStation.getMAC().c_str(), WifiStation.getHostname().c_str());
    startWebServer();
    registerNode();
    nodeHeartBeatInit();
}

// Will be called when WiFi hardware and software initialization was finished
// And system initialization was completed
static void ready()
{
    debugf("READY!");

    ShowInfo();

    // Init wifi
    wifiStart(wifiOk);

    //  --------- EXAMPLE CODE -----------
    shroomInit();
}

void init()
{
    Serial.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL_FULL); // 115200 by default
    Serial.systemDebugOutput(true);                          // Enable debug output to serial
    debugf("Sming. Let's do smart things!");

    // Set system ready callback method
    System.onReady(ready);
}