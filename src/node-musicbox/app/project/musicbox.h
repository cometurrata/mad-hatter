#ifndef MUSIC_BOX_H
#define MUSIC_BOX_H

#include <SmingCore.h>
#include <Libraries/MCP23017/MCP23017.h>

class MusicBoxClass
{
public:
    void init();
    void reset();
    bool isMelodyCorrect();

private:
    void task();
    MCP23017 mcp;
    Timer procTimer;
    bool done = false;

    uint8_t melody[8] = {DO2, SOL, SOL, LA, SOL, SI, DO2, NOTE_MAX};
    uint8_t nextNoteIdx = 0;

    char descriptions[18][10] = {"DO", "DO_SHARP", "RE", "RE_SHARP", "MI", "MI_SHARP", "FA", "FA_SHARP", "SOL", "SOL_SHARP", "LA", "LA_SHARP", "SI", "DO2", "DO2_SHARP", "RE2", "NOTE_MAX"};

    typedef enum Notes
    {
        DO = 0,
        DO_SHARP,
        RE,
        RE_SHARP,
        MI,
        MI_SHARP,
        FA,
        FA_SHARP,
        SOL,
        SOL_SHARP,
        LA,
        LA_SHARP,
        SI,
        DO2,
        DO2_SHARP,
        RE2,
        NOTE_MAX
    } notes_t;
};

extern MusicBoxClass MusicBox;

#endif // MUSIC_BOX_H
