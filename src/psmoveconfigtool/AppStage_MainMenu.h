#ifndef APP_STAGE_INTRO_SCREEN_H
#define APP_STAGE_INTRO_SCREEN_H

//-- includes -----
#include "AppStage.h"

//-- definitions -----
class AppStage_MainMenu : public AppStage
{
public:    
    AppStage_MainMenu(class App *app);

    virtual bool init(int argc, char** argv) override;
    virtual void enter() override;
    virtual void exit() override;

    virtual void renderUI() override;

    virtual bool onClientAPIEvent(
        ClientPSMoveAPI::eEventType event_type,
        ClientPSMoveAPI::t_event_data_handle opaque_event_handle) override;

    static const char *APP_STAGE_NAME;

protected:
    enum eMainMenuState
    {
        inactive,
        connectedToService,
        pendingConnectToToService,
        failedConnectionToService,
        disconnectedFromService,
    };
    eMainMenuState m_menuState;
};

#endif // APP_STAGE_INTRO_SCREEN_H