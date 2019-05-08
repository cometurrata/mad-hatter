#include <user_config.h>
#include <SmingCore/SmingCore.h>

#define MAX_NB_OF_NOTES 15

MCP mcp;
Timer procTimer;

bool done = false;

typedef enum Notes
{
    DO = 0,
    DO_,
    RE,
    RE_,
    MI,
    MI_,
    FA,
    FA_,
    SOL,
    SOL_,
    LA,
    LA_,
    SI,
    SI_,
    DO2,
    DO2_,
    NOTE_MAX
} notes_t;

uint8_t* order = { DO, RE };
uint8_t nextNote = 0;

void task()
{
    if (done)
    {
        return;
    }

    for (int note = 0; note < NOTE_MAX; note++)
    {
        if (mcp.digitalRead(note))
        {
            if (note == order[nextNote])
            {
                nextNote++;
            }
            else
            {
                nextNote = 0;
            }
        }
    }

    if (nextNote == MAX_NB_OF_NOTES)
    {
        done = true;
    }
}

void musicBoxReset()
{
    done = false;
    nextNote = 0;
}

bool musicBoxGetValidated()
{
    return done;
}

void musicBoxInit()
{
    Wire.pins(4, 5); // SDA, SCL

    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // I2C frequency at 400 kHz

    mcp.begin();

    procTimer.initializeMs(20, task).start();
}
