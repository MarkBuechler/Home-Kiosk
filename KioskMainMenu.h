/* */

#ifndef _KIOSK_MAINMENU_H_
#define _KIOSK_MAINMENU_H_

#include <stdio.h>
#include <QtGui>

#include "KioskGUI.h"
#include "KioskBackground.h"
#include "KioskWebBrowser.h"
#include "KioskUtilityMenu.h"
#include "KioskMonitoringMenu.h"
#include "KioskLightingMenu.h"
#include "KioskMediaMenu.h"

class MainMenu : public KioskMenuWidget
{
    Q_OBJECT
public:
    MainMenu(KioskDBLink *link=0);

private slots:
    void slot_lights(void);
    void slot_weather(void);
    void slot_security(void);
    void slot_appliances(void);
    void slot_monitoring(void);
    void slot_browser(void);
    void slot_media(void);
    void slot_utility(void);
    void slot_maps(void);
};

#endif
