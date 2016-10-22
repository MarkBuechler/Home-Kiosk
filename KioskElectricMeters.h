/* */

#ifndef _KIOSK_ELECTRICMETERS_H_
#define _KIOSK_ELECTRICMETERS_H_

#include <QtGui>

#include "KioskGUI.h"
#include "KioskMeterWidget.h"

enum MeterType {
    WATT_METER,
    VOLT_METER,
};

class KioskElectricMeters : public QWidget
{
    Q_OBJECT
public:
    KioskElectricMeters(QWidget *parent=0);

    void addMeter(QString, QString, MeterType);
    void setMeter(QString, int);

protected slots:
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void paintEvent(QPaintEvent*);
    virtual void resizeEvent(QResizeEvent*);

private slots:
    void slot_read_socket(void);

private:
    QVBoxLayout *top;
    QGridLayout *layout;

    QHash<QString, KioskMeterWidget*> meters;

    QTcpSocket *socket;

    KioskHeaderLabel *label;

    int height_hint;

    int last_row;
    int last_column;
};

#endif
