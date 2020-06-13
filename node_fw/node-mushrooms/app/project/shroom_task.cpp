#include <stdint.h>
#include <SmingCore.h>
#include <Libraries/Adafruit_NeoPixel/Adafruit_NeoPixel.h>
#include "http_client.h"

#include "shroom_task.h"
#include "shroom.h"

#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp;

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
#include "tasks.h"

static int combinaisonIndex = 0;
static uint8_t combinaison[5] = {1, 2, 3, 4, 3};

void checkCombinaison(int id)
{
	if (id == combinaison[combinaisonIndex])
	{
		debugf("checkCombinaison index: %d", combinaisonIndex);
		combinaisonIndex ++;
		if (combinaisonIndex >= 4)
		{
			combinaisonIndex = 0;
			nodeMushrooms.setSolved(true);
		}
	}
	else {
		combinaisonIndex = 0;
	}
}

void onTouch(String name)
{
	debugf("touched: %s", name.c_str());
	checkCombinaison(name.toInt());
}

void shroomInit()
{
	Wire.pins(SDA_PIN, SCL_PIN); // SDA, SCL
	mcp.begin();
	shroom1.init(mcp, SHROOM_1_TOUCH, SHROOM_1_PIN, NUMPIXELS, "1", onTouch);
	shroom2.init(mcp, SHROOM_2_TOUCH, SHROOM_2_PIN, NUMPIXELS, "2", onTouch);
	shroom3.init(mcp, SHROOM_3_TOUCH, SHROOM_3_PIN, NUMPIXELS, "3", onTouch);
	shroom4.init(mcp, SHROOM_4_TOUCH, SHROOM_4_PIN, NUMPIXELS, "4", onTouch);
}

