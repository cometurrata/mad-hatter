#ifndef WIFI_H
#define WIFI_H

#include <user_config.h>
#include <SmingCore.h>

class WifiClass
{
public:
    void restart();
    void startConnect();
    void stopConnect();
    bool getConnected();
    void setSSID(String target_ssid);
    void setPassword(String target_ssid);
    String getSSID();
    String getPassword();
    void watchdogTick();
    void setOnDisconnectUserCb(void (*onDisconnectUserCb)());
    void setOnConnectUserCb(void (*onConnectUserCb)());
    void setOnStopUserCb(void (*onStopUserCb)());
    void setOnDisconnectTimeout(void (*onDisconnectTimeout)());
    int8_t getRssi();
    bool getIsEnabled();
    bool ensureConnected();

    uint32_t getStartDate();

private:
    bool is_connected = false;
    bool enabled = false;

    uint32_t startDate;

    String ssid = "";
    String pwd = "";

    uint32_t not_connected_seconds_counter;

    bool scan_done = false;

    Timer watchdogTimer;

    void onConnectOk(IpAddress ip, IpAddress mask, IpAddress gateway);
    void onConnectFail(String ssid, uint8_t ssidLength, uint8_t *bssid, uint8_t reason);
    //void onDisconnected(String ssid, uint8_t ssidLength, uint8_t *bssid, uint8_t reason);
    void onDisconnected(const String &ssid, MacAddress bssid, WifiDisconnectReason reason);
    void configureWifi();

    void (*onDisconnectTimeout)() = NULL;
    void (*onDisconnectUserCb)() = NULL;
    void (*onConnectUserCb)() = NULL;
    void (*onStopUserCb)() = NULL;
};
extern WifiClass Wifi;
#endif