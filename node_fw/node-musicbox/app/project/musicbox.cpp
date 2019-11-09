#include "musicbox.h"

MusicBoxClass MusicBox;

#define MAX_NB_OF_NOTES 15

void MusicBoxClass::task()
{
    if (done)
    {
        return;
    }

    for (int note = 0; note < NOTE_MAX; note++)
    {
        if (mcp.digitalRead(note))
        {
            if (note == melody[nextNote])
            {
                nextNote++;
            }
            else
            {
                nextNote = 0;
            }
        }
    }

    if (nextNote == sizeof(melody))
    {
        done = true;
    }
}

void MusicBoxClass::reset()
{
    done = false;
    nextNote = 0;
}

bool MusicBoxClass::isMelodyCorrect()
{
    return done;
}

void MusicBoxClass::init()
{
    Wire.pins(4, 5); // SDA, SCL
    mcp.begin();
    procTimer.initializeMs(20, std::bind(&MusicBoxClass::task, this)).start();
}
