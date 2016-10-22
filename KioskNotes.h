/* */

#ifndef _KIOSK_NOTES_H_
#define _KIOSK_NOTES_H_

#include <QtGui>
#include <QtOpenGL>

#include "KioskGUI.h"

class KioskNotes : public KioskWidget
{
    Q_OBJECT
public:
    KioskNotes(void);

protected:
    virtual void paintEvent(QPaintEvent*);
};

#endif
