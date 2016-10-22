/* */

#ifndef _KIOSK_CLOCKWIDGET_H_
#define _KIOSK_CLOCKWIDGET_H_

#include <cmath>

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QDateTime>
#include <QTimer>
#include <QHash>
#include <QEvent>
#include <QPropertyAnimation>

class KioskClockWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
public:
    KioskClockWidget(QWidget *parent=0);

    void setShaded(QHash<int,int>);

protected slots:
    virtual void changeEvent(QEvent*);
    virtual void paintEvent(QPaintEvent*);

private slots:
    void updateClock(void);

private:
    qreal rotation(void) {return m_angle;}
    void setRotation(qreal);

    QPixmap m_clockFace;

    QPropertyAnimation *m_animation;
    QTimer *m_timer;

    qreal m_angle;

    QHash<int,int> m_shades;

    bool m_enabled;
};

#endif
