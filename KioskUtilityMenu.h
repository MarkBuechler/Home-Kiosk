/* */

#ifndef _KIOSK_UTILITYMENU_H_
#define _KIOSK_UTILITYMENU_H_

#include <stdio.h>
#include <QtGui>

#include "KioskGUI.h"
#include "KioskCalendar.h"
#include "KioskQmlItems.h"

class UtilityMenu : public KioskMenuWidget
{
    Q_OBJECT
public:
    UtilityMenu(void);

private slots:
    void slot_calendar(void);
    void slot_photoviewer(void);
    void slot_newsviewer(void);
    void slot_exit(void);
};

#endif
