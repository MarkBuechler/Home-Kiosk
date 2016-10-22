/* */

#ifndef _KIOSK_DEBUG_H_
#define _KIOSK_DEBUG_H_

#include <QtGui>

void kdebug(QString message)
{
    qDebug(qPrintable(message));
}

#endif
