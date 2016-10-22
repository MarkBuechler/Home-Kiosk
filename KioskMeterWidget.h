/* */

#ifndef _KIOSK_METERWIDGET_
#define _KIOSK_METERWIDGET_

#include <cmath>

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QDebug>

#define WATT_RANGE_SIZE 34

static const float watt_range1[][2] = {
    {0.0, 0.0},      {8.8, 60.0},    {12.5, 70.0},    {15.0, 80.0},
    {17.75, 90.0},   {20.0, 100.0},  {24.65, 150.0},  {28.7, 200.0},
    {32.5, 250.0},   {35.7, 300.0},  {38.8, 350.0},   {41.7, 400.0},
    {44.3, 450.0},   {46.5, 500.0},  {49.7, 550.0},   {51.1, 600.0},
    {53.3, 650.0},   {55.6, 700.0},  {58.0, 750.0},   {60.0, 800.0},
    {62.0, 850.0},   {64.2, 900.0},  {66.3, 950.0},   {68.0, 1000.0},
    {72.5, 1100.0},  {76.7, 1200.0}, {80.6, 1300.0},  {84.5, 1400.0},
    {87.7, 1500.0},  {91.3, 1600.0}, {94.5, 1700.0},  {97.5, 1800.0},
    {99.6, 1900.0},  {102.5, 2000.0},
};

static const float watt_range2[][2] = {
    {21.7, 1000.0},  {26.5, 1500.0},   {30.5, 2000.0},
    {34.3, 2500.0},  {37.7, 3000.0},   {40.6, 3500.0},
    {43.4, 4000.0},  {46.0, 4500.0},   {48.6, 5000.0},
    {51.0, 5500.0},  {53.0, 6000.0},   {55.0, 6500.0},
    {57.5, 7000.0},  {59.0, 7500.0},   {61.5, 8000.0},
    {63.5, 8500.0},  {65.5, 9000.0},   {67.5, 9500.0},
    {70.0, 10000.0}, {74.0, 11000.0},  {77.5, 12000.0},
    {81.6, 13000.0}, {85.1, 14000.0},  {88.5, 15000.0},
    {91.5, 16000.0}, {95.0, 17000.0},  {97.5, 18000.0},
    {99.6, 19000.0}, {102.5, 20000.0}, {0.0, 0.0},
    {0.0, 0.0},      {0.0, 0.0},       {0.0, 0.0},
    {0.0, 0.0},
};

#define VOLT_RANGE_SIZE 51

static const float volt_range1[][2] = {
    {0.5, 0.0},    {3.0, 5.0},    {4.6, 10.0},   {6.6, 15.0},
    {8.4, 20.0},   {10.3, 25.0},  {12.0, 30.0},  {12.0, 35.0},
    {16.0, 40.0},  {18.0, 45.0},  {19.5, 50.0},  {21.5, 55.0},
    {23.5, 60.0},  {25.5, 65.0},  {27.0, 70.0},  {28.8, 75.0},
    {30.8, 80.0},  {32.8, 85.0},  {34.8, 90.0},  {36.0, 95.0},
    {38.0, 100.0}, {39.8, 105.0}, {41.6, 110.0}, {43.6, 115.0},
    {45.5, 120.0}, {46.5, 125.0}, {40.0, 130.0}, {50.0, 135.0},
    {51.5, 140.0}, {53.5, 145.0}, {55.5, 150.0}, {57.0, 155.0},
    {59.0, 160.0}, {60.5, 165.0}, {62.5, 170.0}, {64.5, 175.0},
    {65.8, 180.0}, {67.8, 185.0}, {69.8, 190.0}, {71.8, 195.0},
    {73.3, 200.0}, {75.3, 205.0}, {77.2, 210.0}, {78.9, 215.0},
    {80.9, 220.0}, {82.4, 225.0}, {84.4, 230.0}, {86.0, 235.0},
    {88.0, 240.0}, {90.0, 245.0}, {91.7, 250.0},
};

enum MeterRange {
    WATT_AUTO,
    VOLT_AUTO,
    WATT_RANGE1,
    WATT_RANGE2,
    VOLT_RANGE1,
};

class KioskMeterWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
public:
    KioskMeterWidget(QWidget *parent=0);

    virtual QSize sizeHint(void);

    virtual void setMeterValue(qreal) {}
    virtual void setValue(qreal);
    int value(void) {return m_value;}

    void setMeterRange(qreal, qreal);

    void setLabel(QString label);

    void setRotation(qreal);
    qreal rotation(void) {return m_angle;}

protected:
    void setNeedlePixmaps(QPixmap, QPixmap);
    void setBackgroundPixmap(QPixmap);
    void setOverlayPixmap(QPixmap);
    void setLedPixmap(QPixmap);
    void setNeedleOffset(QPoint);
    void setNeedleShadowOffset(QPoint);
    void setOverlayOffset(QPoint);
    void setLedOffset(QPoint);
    void setLabelRect(QRectF);
    void setNeedleOrigins(QPoint, QPoint);
    void setRange(qreal, qreal);

protected slots:
    virtual void mousePressEvent(QMouseEvent*);
    virtual void paintEvent(QPaintEvent*);

private:
    qreal m_value;
    qreal m_angle;
    qreal m_rangeLow;
    qreal m_rangeHigh;

    QString m_label;

    QPixmap m_background;
    QPixmap m_overlay;
    QPixmap m_needle;
    QPixmap m_shadow;
    QPixmap m_led;

    QPoint m_needleOffset;
    QPoint m_needleOrigin;
    QPoint m_shadowOrigin;
    QPoint m_shadowOffset;
    QPoint m_overlayOffset;
    QPoint m_ledOffset;

    QRectF m_labelRect;

    QPropertyAnimation *m_animation;
    QEasingCurve m_curve;
};

class KioskWattMeter : public KioskMeterWidget
{
    Q_OBJECT
public:
    KioskWattMeter(QWidget *parent=0);

    virtual void setValue(qreal);
    void setRange(MeterRange range=WATT_AUTO);
    void setMeterValue(qreal);

private:
    MeterRange m_currentRange;

    QPixmap m_background;
    QPixmap m_overlay;
    QPixmap m_needle;
    QPixmap m_shadow;
    QPixmap m_led;
    QPixmap m_dled;
    QPixmap m_lled;

    qreal m_currentPercent;
};

class KioskVoltMeter : public KioskMeterWidget
{
    Q_OBJECT
public:
    KioskVoltMeter(QWidget *parent=0);

    virtual void setValue(qreal);
    void setRange(MeterRange range=VOLT_AUTO);
    void setMeterValue(qreal);

private:
    MeterRange m_currentRange;

    QPixmap m_background;
    QPixmap m_overlay;
    QPixmap m_needle;
    QPixmap m_shadow;

    qreal m_currentPercent;
};

#endif
