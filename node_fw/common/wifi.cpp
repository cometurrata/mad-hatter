#include "wifi.h"

#define TIME_BEFORE_RESET 20

WifiClass Wifi;

bool WifiClass::ensureConnected()
{
    if (this->getConnected())
    {
        debugf("Wifi connected ");
        return true;
    }
    else
    {
        debugf("Wifi not connected");
        this->startConnect();
        return false;
    }
}

String WifiClass::getSSID()
{
    return ssid;
}

String WifiClass::getPassword()
{
    return pwd;
}

void WifiClass::setSSID(String target)
{
    ssid = target;
}

void WifiClass::setPassword(String target)
{
    pwd = target;
}

int8_t WifiClass::getRssi()
{
    return WifiStation.getRssi();
}

bool WifiClass::getIsEnabled()
{
    return enabled;
}

void WifiClass::restart()
{
    WifiEvents.onStationDisconnect(StationDisconnectDelegate(&WifiClass::onDisconnected, this));
    WifiStation.disconnect();
    WifiStation.enable(false);
    configureWifi();
}

void WifiClass::watchdogTick()
{
    if (!WifiStation.isConnected())
    {
        debugf("not connected to Data");
        not_connected_seconds_counter++;
        if (not_connected_seconds_counter > TIME_BEFORE_RESET)
        {
            if (onDisconnectTimeout != NULL)
            {
                onDisconnectTimeout();
            }
            debugf("restart");
            restart();
        }
    }
    else
    {
        not_connected_seconds_counter = 0;
    }
}

bool WifiClass::getConnected()
{
    return is_connected;
}

void WifiClass::startConnect()
{
    if (enabled == false)
    {
        startDate = millis();
        enabled = true;
        configureWifi();
        watchdogTimer.initializeMs(1000, std::bind(&WifiClass::watchdogTick, this)).start();
    }
    else
    {
        debugf("WifiClass::startConnect already started");
    }
}

uint32_t WifiClass::getStartDate()
{
    return startDate;
}

void WifiClass::stopConnect()
{
    if ((onStopUserCb != NULL) && (enabled == true))
    {
        onStopUserCb();
    }
    enabled = false;
}

Timer t;

void WifiClass::onConnectOk(IpAddress ip, IpAddress mask, IpAddress gateway)
{
    debugf("I'm CONNECTED");
    //debugf(ip.toString());
    debugf("Try first log connection");
    is_connected = true;
    if ((onConnectUserCb != NULL) && (enabled == true))
    {
        onConnectUserCb();
    }
}

void WifiClass::onConnectFail(String ssid, uint8_t ssidLength, uint8_t *bssid, uint8_t reason)
{
    if ((onDisconnectUserCb != NULL) && (is_connected == true))
    {
        onDisconnectUserCb();
    }

    is_connected = false;
    debugf("%s from %s. Reason: %d", __FUNCTION__, ssid.c_str(), reason);
    if (enabled == true)
    {
        restart();
    }
    else
    {
        WifiStation.disconnect();
    }
}

void WifiClass::onDisconnected(const String &ssid, MacAddress bssid, WifiDisconnectReason reason)
{
    if ((onDisconnectUserCb != NULL) && (is_connected == true))
    {
        onDisconnectUserCb();
    }

    is_connected = false;

    if (enabled == true)
    {
        debugf("%s from %s. Reason: %d", __FUNCTION__, ssid.c_str(), reason);
        configureWifi();
    }
}

void WifiClass::configureWifi()
{

    WifiEvents.onStationGotIP(StationGotIPDelegate(&WifiClass::onConnectOk, this));
    WifiEvents.onStationDisconnect(StationDisconnectDelegate(&WifiClass::onDisconnected, this));

    debugf("configureWifi ...");
    not_connected_seconds_counter = 0;

    if (!WifiStation.isEnabled())
    {
        WifiStation.enable(true);
    }
    if (ssid != "")
    {
        WifiStation.config(ssid, pwd);
    }
    WifiStation.connect();
}

void WifiClass::setOnDisconnectUserCb(void (*onDisconnectCallBack)(void))
{
    this->onDisconnectUserCb = onDisconnectCallBack;
}

void WifiClass::setOnConnectUserCb(void (*onConnectCallBack)(void))
{
    this->onConnectUserCb = onConnectCallBack;
}

void WifiClass::setOnStopUserCb(void (*onStopCallBack)(void))
{
    this->onStopUserCb = onStopCallBack;
}

void WifiClass::setOnDisconnectTimeout(void (*onDisconnectTimeoutCallBack)(void))
{
    this->onDisconnectTimeout = onDisconnectTimeoutCallBack;
}