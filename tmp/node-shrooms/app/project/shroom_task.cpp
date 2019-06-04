#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Adafruit_NeoPixel/Adafruit_NeoPixel.h>
#include "http_client.h"

#include "project/shroom_task.h"
#include "project/shroom.h"

#include "Adafruit_MCP23017.h"


#define SDA_PIN 4
#define SCL_PIN 5

#define D8 15
#define D7 13
#define D6 12
#define D5 14
#define D0 16
#define D4 2
#define D3 0
#define D2 4
#define D1 5
#define RX 1

// How many NeoPixels are attached to the Esp8266?
#define NUMPIXELS 10

Shroom shroom1;
Shroom shroom2;
Shroom shroom3;
Shroom shroom4;

#define SHROOM_1_PIN D7
#define SHROOM_2_PIN D5
#define SHROOM_3_PIN D6
#define SHROOM_4_PIN D3

#define SHROOM_1_TOUCH 0
#define SHROOM_2_TOUCH 1
#define SHROOM_3_TOUCH 2
#define SHROOM_4_TOUCH 3

#include "shroom.h"

static Timer heartBeatTimer;

void nodeHeartBeatInit()
{
    heartBeatTimer.initializeMs(30 * 1000, [] { sendHeartBeat(); }).start();
}

void shroomInit()
{
    Wire.pins(SDA_PIN, SCL_PIN); // SDA, SCL
    mcp.begin();
	shroom1.init(SHROOM_1_TOUCH, SHROOM_1_PIN, NUMPIXELS, "1");
	shroom2.init(SHROOM_2_TOUCH, SHROOM_2_PIN, NUMPIXELS, "2");
	shroom3.init(SHROOM_3_TOUCH, SHROOM_3_PIN, NUMPIXELS, "3");
	shroom4.init(SHROOM_4_TOUCH, SHROOM_4_PIN, NUMPIXELS, "4");
}
