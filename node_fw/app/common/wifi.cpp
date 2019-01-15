#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "common/wifi.h"
#include "common/node_register.h"
#include "project/http_server.h"

//#define USE_SMART_CONFIG
#ifndef USE_SMART_CONFIG
#define WIFI_SSID "MY_WIFI"
#define WIFI_PASSWORD "MY_PASSWORD"
#endif

// Will be called when WiFi station network scan was completed
static void listNetworks(bool succeeded, BssList list)
{
	if(!succeeded) {
		Serial.println("Failed to scan networks");
		return;
	}

	for(int i = 0; i < list.count(); i++) {
		Serial.print("\tWiFi: ");
		Serial.print(list[i].ssid);
		Serial.print(", ");
		Serial.print(list[i].getAuthorizationMethodName());
		if(list[i].hidden)
			Serial.print(" (hidden)");
		Serial.println();
	}
}

// Will be called when WiFi station was connected to AP
static void connectOk(IPAddress ip, IPAddress mask, IPAddress gateway)
{
	debugf("AP. ip: %s mac: %s hostname: %s", WifiStation.getIP().toString().c_str(), WifiStation.getMAC().c_str(), WifiStation.getHostname().c_str());
	startWebServer();
	registerNode();
}

// Will be called when WiFi station was disconnected
static void connectFail(String ssid, uint8_t ssidLength, uint8_t* bssid, uint8_t reason)
{
	// The different reason codes can be found in user_interface.h. in your SDK.
	debugf("Disconnected from %s. Reason: %d", ssid.c_str(), reason);
}

struct WifiConfig {
        String ssid;
	String password;
};

#define WIFI_CONFIG_FILE ".wifi.conf" // leading point for security reasons :)

static int loadConfig(struct WifiConfig *cfg)
{
	DynamicJsonBuffer jsonBuffer;
	if(fileExist(WIFI_CONFIG_FILE)) {
		int size = fileGetSize(WIFI_CONFIG_FILE);
		char* jsonString = new char[size + 1];
		fileGetContent(WIFI_CONFIG_FILE, jsonString, size + 1);

                JsonObject& root = jsonBuffer.parseObject(jsonString);
		cfg->ssid = String((const char*)root["ssid"]);
		cfg->password = String((const char*)root["password"]);

                debugf("[WIFI] Loaded Wifi confib %s:%s\n", (const char*)root["ssid"], (const char*)root["password"]);

		delete[] jsonString;
                return 0;
	} else {
#ifndef USE_SMART_CONFIG
		cfg->ssid = String(WIFI_SSID);
		cfg->password = String(WIFI_PASSWORD);
		return 0;
#else
		return -1;
#endif
	}
}

void saveConfig(struct WifiConfig *cfg)
{
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	JsonObject& network = jsonBuffer.createObject();
	root["ssid"] = cfg->ssid.c_str();
	root["password"] = cfg->password.c_str();

        debugf("[WIFI] Saving wifi config %s:%s to file\n", root["ssid"], root["password"]);

	char buf[1024];
	root.prettyPrintTo(buf, sizeof(buf));
        //debugf("%s", buf);
	fileSetContent(WIFI_CONFIG_FILE, buf);
}

static void smartConfigCallback(sc_status status, void* pdata)
{
	switch(status) {
	case SC_STATUS_WAIT:
		debugf("SC_STATUS_WAIT\n");
		break;
	case SC_STATUS_FIND_CHANNEL:
		debugf("SC_STATUS_FIND_CHANNEL\n");
		break;
	case SC_STATUS_GETTING_SSID_PSWD:
		debugf("SC_STATUS_GETTING_SSID_PSWD\n");
		break;
	case SC_STATUS_LINK: {
		debugf("SC_STATUS_LINK\n");
		station_config* sta_conf = (station_config*)pdata;
		char* ssid = (char*)sta_conf->ssid;
		char* password = (char*)sta_conf->password;

                struct WifiConfig cfg = {
                        .ssid = String(ssid),
                        .password = String(password)
                };
                saveConfig(&cfg);

		WifiStation.config(ssid, password);
		WifiStation.connect();
	} break;
	case SC_STATUS_LINK_OVER:
		debugf("SC_STATUS_LINK_OVER\n");
		WifiStation.smartConfigStop();
		break;
	}
}

static void startConnection(void)
{
        struct WifiConfig cfg;

        // Config already saved, use it
        if (loadConfig(&cfg) == 0) {
                debugf("[WIFI] Config file found, connecting to WIFI %s:%s\n", cfg.ssid.c_str(), cfg.password.c_str());
                WifiStation.config(cfg.ssid, cfg.password);
                WifiStation.connect();
        }
        else {
                debugf("[WIFI] Config file not found, starting SmartConfig\n");
                // Start SmartConfig
                WifiStation.smartConfigStart(SCT_EspTouch, smartConfigCallback);
        }
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
