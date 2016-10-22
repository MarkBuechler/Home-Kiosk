/* */

#ifndef _TEST_METERS_H_
#define _TEST_METERS_H_

#include <QtGui>

#include "KioskGUI.h"
#include "KioskMeterWidget.h"
#include "KioskSlideWidget.h"
#include "KioskClockWidget.h"

class TestMeters : public KioskWidget
{
    Q_OBJECT
public:
    TestMeters(QWidget *parent=0);

private slots:
    void changeMeter(void);
    virtual void paintEvent(QPaintEvent*);

private:
    KioskMeterWidget *dial;
    QLineEdit *edit;
    QLineEdit *edit2;
    QLineEdit *edit3;
};

#endif
