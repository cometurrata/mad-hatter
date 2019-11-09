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
};

extern MusicBoxClass MusicBox;

#endif // MUSIC_BOX_H
