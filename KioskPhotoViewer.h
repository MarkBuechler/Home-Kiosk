/* */

#ifndef _KIOSK_PHOTOVIEWER_H_
#define _KIOSK_PHOTOVIEWER_H_

#include <QtGui>
#include <QtOpenGL>

#include "KioskGUI.h"

class KioskPhotoViewer : public KioskWidget
{
    Q_OBJECT
public:
    KioskPhotoViewer(void);

protected:
    virtual void paintEvent(QPaintEvent*);
};

#endif
