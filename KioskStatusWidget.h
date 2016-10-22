/* */

#ifndef _KIOSK_STATUSWIDGET_H_
#define _KIOSK_STATUSWIDGET_H_

#include <QtGui>

#include "KioskGUI.h"
#include "KioskElectricMeters.h"
#include "KioskWebBrowser.h"
#include "KioskClockWidget.h"

class KioskStatusWidget : public KioskWidget
{
    Q_OBJECT
public:
    KioskStatusWidget(QWidget *parent=0);

private:
    KioskWebView *web;
};

#endif
