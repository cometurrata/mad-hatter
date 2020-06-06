#include <SmingCore.h>

#include "wifi.h"
#include "node_register.h"
#include "http_server.h"

// Will be called when WiFi station was connected to AP
void connectOk(IpAddress ip, IpAddress mask, IpAddress gateway)
{
    Serial.print(_F("I'm CONNECTED to "));
    Serial.println(ip);
    debugf("AP. ip: %s mac: %s hostname: %s", WifiStation.getIP().toString().c_str(), WifiStation.getMAC().c_str(), WifiStation.getHostname().c_str());
    startWebServer();
    registerNode();
}

// Will be called when WiFi station was disconnected
void connectFail(const String &ssid, MacAddress bssid, WifiDisconnectReason reason)
{
    // The different reason codes can be found in user_interface.h. in your SDK.
    Serial.print(_F("Disconnected from \""));
    Serial.print(ssid);
    Serial.print(_F("\", reason: "));
    Serial.println(WifiEvents.getDisconnectReasonDesc(reason));
}

static void startConnection(void)
{
    WifiStation.config(WIFI_SSID, WIFI_PASSWORD);
    WifiStation.connect();
    debugf("Connecting to %s, %s", WIFI_SSID, WIFI_PASSWORD);
}

void wifiStart(StationGotIPDelegate cb)
{
    // Access Point disabled
    WifiAccessPoint.enable(false);

    // Station - WiFi client
    WifiStation.enable(true);

    // Set callback that should be triggered when we have assigned IP
    if (cb == null) {
        cb = connectOk;
    }
    WifiEvents.onStationGotIP(cb);

    // Set callback that should be triggered if we are disconnected or connection attempt failed
    WifiEvents.onStationDisconnect(connectFail);

    startConnection();
}
