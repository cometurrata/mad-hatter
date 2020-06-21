#include "ui_controller.h"

// #define UI_CONTROLLER_DEBUG

#ifdef UI_CONTROLLER_DEBUG
#define uictrl_dbg(msg...) Serial.print(msg)
#define uictrl_dbgln(msg...) Serial.println(msg)
#else
#define uictrl_dbg(msg...)
#define uictrl_dbgln(msg...)
#endif

#define SUCCESS_ANIM_NB_BLINKS        3
#define SUCCESS_ANIM_STEP_DELAY_MS    300
#define PASSWORD_STEP_DELAY_MS        1000

void UIController::startUITimer(int timeout)
{
    timer.initializeMs(timeout, std::bind(&UIController::uiTask, this)).startOnce();
}

void UIController::displaySuccessAnimation()
{
    uictrl_dbg("[UI] Show success anim (step = ");
    uictrl_dbg(animStep);
    uictrl_dbgln(")");

    if (animStep % 2 == 0) {
        ledController->turnOnAll();
    }
    else {
        ledController->turnOffAll();
    }

    animStep++;

    // Success animation is over, display password
    if (animStep >= 2*SUCCESS_ANIM_NB_BLINKS) {
        animStep = 0;
        state = STATE_DISPLAY_PASSWORD;
        startUITimer(PASSWORD_STEP_DELAY_MS);
    }
    else {
        startUITimer(SUCCESS_ANIM_STEP_DELAY_MS);
    }
}

void UIController::displayPassword()
{
    uictrl_dbg("[UI] Show password (step = ");
    uictrl_dbg(animStep);
    uictrl_dbgln(")");

    if (animStep >= PASSWORD_SIZE) {
        // If done showing password, turnoff leds
        if (ledController) {
            uictrl_dbgln("[UI] Turn LEDs OFF");
            ledController->turnOff();
        }

        uictrl_dbgln("[UI] Animation END");
        animStep = 0;
        state = STATE_DISPLAY_SUCCESS_ANIM;
        if (onPasswordEndDisplayCb) {
            onPasswordEndDisplayCb();
            onPasswordEndDisplayCb = nullptr;
        }
    }
    else {
        if (ledController) {
            ledController->turnOff();
            uictrl_dbg("[UI] Turn LED ");
            uictrl_dbg(password[animStep]);
            uictrl_dbgln(" ON");
            ledController->turnOn(password[animStep]);
        }

        animStep++;
        startUITimer(PASSWORD_STEP_DELAY_MS);
    }
}

void UIController::uiTask()
{
    switch(state) {
    case STATE_DISPLAY_SUCCESS_ANIM:
        displaySuccessAnimation();
        break;

    case STATE_DISPLAY_PASSWORD:
        displayPassword();
        break;
    }
}

void UIController::startShowingPassword(UIController::OnPasswordEndDisplayCb cb)
{
    onPasswordEndDisplayCb = cb;
    startUITimer(10);
}

void UIController::init(LedController *ledController)
{
    this->ledController = ledController;
}