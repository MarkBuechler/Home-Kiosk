/* */

#ifndef _KIOSK_BACKGROUND_H_
#define _KIOSK_BACKGROUND_H_

#include "stdio.h"

#include <QtGui>
#include <QX11EmbedContainer>

class KioskBackground : public QWidget
{
    Q_OBJECT
public:
    KioskBackground(void);
    ~KioskBackground(void);

    void setApp(QString app);

private:
    QProcess *process;
    QX11EmbedContainer *container;
};

#endif
