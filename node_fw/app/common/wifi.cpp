#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "common/wifi.h"
#include "common/node_register.h"
#include "project/http_server.h"

// Will be called when WiFi station was connected to AP
static void connectOk(IPAddress ip, IPAddress mask, IPAddress gateway)
{
    debugf("AP. ip: %s mac: %s hostname: %s", WifiStation.getIP().toString().c_str(), WifiStation.getMAC().c_str(), WifiStation.getHostname().c_str());
    startWebServer();
    registerNode();
}

// Will be called when WiFi station was disconnected
static void connectFail(String ssid, uint8_t ssidLength, uint8_t *bssid, uint8_t reason)
{
    // The different reason codes can be found in user_interface.h. in your SDK.
    debugf("Disconnected from %s. Reason: %d", ssid.c_str(), reason);
}

static void startConnection(void)
{
    WifiStation.config(WIFI_SSID, WIFI_PASSWORD);
    WifiStation.connect();
    debugf("Connecting to %s, %s", WIFI_SSID, WIFI_PASSWORD);
}

void wifiStart(void)
{
    // Access Point disabled
    WifiAccessPoint.enable(false);

    // Station - WiFi client
    WifiStation.enable(true);

    // Set callback that should be triggered when we have assigned IP
    WifiEvents.onStationGotIP(connectOk);

    // Set callback that should be triggered if we are disconnected or connection attempt failed
    WifiEvents.onStationDisconnect(connectFail);

    startConnection();
}
