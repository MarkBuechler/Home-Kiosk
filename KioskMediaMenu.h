/* */

#ifndef _KIOSK_MEDIAMENU_H_
#define _KIOSK_MEDIAMENU_H_

#include <stdio.h>
#include <QtGui>

#include "KioskGUI.h"
#include "KioskJukebox.h"

class MediaMenu : public KioskMenuWidget
{
    Q_OBJECT
public:
    MediaMenu(KioskDBLink *link=0);

private slots:
    void slot_mythtv(void);
    void slot_netflix(void);
    void slot_jukebox(void);
};

#endif
