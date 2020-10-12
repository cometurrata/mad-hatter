#include "musicbox.h"

MusicBoxClass MusicBox;

#define MAX_NB_OF_NOTES 15

#define ON 0
#define OFF 1

void MusicBoxClass::task()
{
    if (done)
    {
        return;
    }

    for (int note = 0; note < NOTE_MAX; note++)
    {

        if (mcp.digitalRead(note) == ON)
        {
            Serial.printf("%s\n", descriptions[note]);
            if (note == melody[nextNoteIdx])
            {
                Serial.println("Note is correct!");
                nextNoteIdx++;
            }
            else if (note >= 1)
            {
                if (note != melody[nextNoteIdx - 1])
                {
                    Serial.println("Note is bad... Reset!");
                    nextNoteIdx = 0;
                }
            }
        }
    }

    if (melody[nextNoteIdx] == NOTE_MAX)
    {
        Serial.println("Done successfully !");
        done = true;
    }
}

void MusicBoxClass::reset()
{
    done = false;
    nextNoteIdx = 0;
}

bool MusicBoxClass::isMelodyCorrect()
{
    return done;
}

void MusicBoxClass::init()
{
    Wire.pins(4, 5); // SDA, SCL
    mcp.begin();
    for (int i = 0; i < 16; i++)
    {
        mcp.pinMode(i, INPUT);
        mcp.pullUp(i, 1);
    }
    procTimer.initializeMs(20, std::bind(&MusicBoxClass::task, this)).start();
}
