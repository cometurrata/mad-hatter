#include <string.h>

#include <SmingCore.h>

#include "wifi.h"
#include "node_register.h"
#include "http_server.h"
#include "http_client.h"
//  --------- EXAMPLE CODE -----------
#include "project/tasks.h"
#include "node.h"

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

void onWifiOk()
{
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

    NodeClock.addNodeType(Node::NodeTypeEnum::SENSOR_)
        .setHostname(NODE_HOSTNAME)
        .start();

    //  --------- EXAMPLE CODE -----------
    nodeClockInit();
}

void init()
{
    Serial.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL_FULL); // 115200 by default
    Serial.systemDebugOutput(true);                          // Enable debug output to serial
    Serial.println("Sming. Let's do smart things!");

    // Set system ready callback method
    System.onReady(ready);
}