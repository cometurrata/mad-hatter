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

    uint8_t melody[3] = {DO, RE, NOTE_MAX};
    uint8_t nextNote = 0;

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
        SI_SHARP,
        DO2,
        DO2_SHARP,
        NOTE_MAX
    } notes_t;
};

extern MusicBoxClass MusicBox;

#endif // MUSIC_BOX_H
