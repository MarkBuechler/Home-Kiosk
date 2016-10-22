/* */

#ifndef _KIOSK_MONITORINGMENU_H_
#define _KIOSK_MONITORINGMENU_H_

#include <stdio.h>
#include <QtGui>

#include "KioskGUI.h"
#include "KioskWebBrowser.h"
#include "KioskStatusWidget.h"

class MonitoringMenu : public KioskMenuWidget
{
    Q_OBJECT
public:
    MonitoringMenu(void);

private slots:
    void slot_electric(void);
    void slot_network(void);
    void slot_status(void);
    void slot_thermostat(void);
};

#endif
