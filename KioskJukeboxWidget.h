/* */

#ifndef _KIOSK_JUKEBOX_WIDGET_H_
#define _KIOSK_JUKEBOX_WIDGET_H_

#include <QtGui>

#include "KioskGUI.h"
#include "KioskJukeboxItem.h"

class KioskJukeboxWidget : public KioskScrollArea
{
    Q_OBJECT
public:
    KioskJukeboxWidget(void);

    void setItem(int, int, KioskJukeboxItem*);

private:
    QGridLayout *m_layout;
};

#endif

