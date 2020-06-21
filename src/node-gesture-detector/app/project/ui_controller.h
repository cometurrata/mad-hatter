#pragma once

#include <SmingCore.h>
#include <Delegate.h>
#include "ledcontroller.h"

class UIController
{
public:
    using OnPasswordEndDisplayCb = Delegate<void()>;   ///< Delegate callback

    void init(LedController *ledController);
    void startShowingPassword(OnPasswordEndDisplayCb cb);

private:
    enum UIState {
        STATE_DISPLAY_SUCCESS_ANIM,
        STATE_DISPLAY_PASSWORD
    };

    #define PASSWORD_SIZE  4
    const uint8_t password[PASSWORD_SIZE] = {LED_LEFT, LED_RIGHT, LED_UP, LED_DOWN};

    LedController *ledController = nullptr;
    OnPasswordEndDisplayCb onPasswordEndDisplayCb = nullptr;
    enum UIState state = STATE_DISPLAY_SUCCESS_ANIM;
    int animStep = 0;
    Timer timer;

    void startUITimer(int timeout);
    void displaySuccessAnimation();
    void displayPassword();
    void uiTask();
};