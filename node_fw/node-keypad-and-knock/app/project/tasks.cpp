#include <stdint.h>
#include <SmingCore.h>

#include "http_client.h"
#include "keypad.h"
#include "tasks.h"

Node nodeKeyPadAndKnock;

static Timer sensorTimer;

void nodeKeypadInit(void)
{
    keyPad.init();
}
