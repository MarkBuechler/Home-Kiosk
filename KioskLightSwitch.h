/* */

#ifndef _KIOSK_LIGHTSWITCH_H_
#define _KIOSK_LIGHTSWITCH_H_

#include "KioskButton.h"

class KioskLightSwitch : public KioskButtonBase
{
    Q_OBJECT
public:
    KioskLightSwitch(QWidget *parent=0);
    virtual void setChecked(bool, bool emitter=true);

protected:
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void paintEvent(QPaintEvent*);

private:
    QPixmap m_background;
    QPixmap m_switchOn;
    QPixmap m_switchOff;

    bool m_checked;
};

class KioskLightDimmer : public KioskButtonBase
{
    Q_OBJECT
public:
    KioskLightDimmer(QWidget *parent=0);

    void setDimmer(int);
    int dimmer(void);

protected slots:
    void animateSlider(void);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void paintEvent(QPaintEvent*);

private:
    QPixmap m_background;
    QPixmap m_button;
    QPixmap m_slider;
    QPixmap m_blueled;
    QPixmap m_greenled;
    QPoint  m_pressPos;
    QPoint  m_movePos;

    QTimer *m_animateTimer;

    int  m_animateDir;
    int  m_dimmer;
    int  m_oldDimmer;
    int  m_positionY;
    int  m_oldPositionY;
    bool m_buttonPressed;
    bool m_sliderPressed;
    bool m_animating;

signals:
    void sliderMoved(int value);
    void sliderChanged(int value);
};

#endif
