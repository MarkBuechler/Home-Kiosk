/* */

#ifndef _KIOSK_NEWSVIEWER_H_
#define _KIOSK_NEWSVIEWER_H_

#include <QtGui>
#include <QtOpenGL>

#include "KioskGUI.h"

class KioskNewsViewer : public KioskWidget
{
    Q_OBJECT
public:
    KioskNewsViewer(void);

protected:
    virtual void paintEvent(QPaintEvent*);
};

#endif
