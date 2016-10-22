/* */

#ifndef _KIOSK_SLIDEWIDGET_H_
#define _KIOSK_SLIDEWIDGET_H_

#include <math.h>

#include <QWidget>
#include <QStyle>
#include <QPainter>
#include <QMouseEvent>
#include <QStyleOptionFrameV2>

#include <QDebug>

enum GradientDirection {
    NoGradient,
    DarkToLight,
    LightToDark,
};

enum SliderNumber {
    SLIDER1,
    SLIDER2,
};

class KioskSlideWidget : public QWidget
{
    Q_OBJECT
public:
    KioskSlideWidget(QWidget *parent=0);

    void setRange(int, int, qreal increment=1.0, int spacing=1);
    void setDirection(GradientDirection direction);

    void setDualSliders(bool set);

    virtual QSize sizeHint(void) const;
    virtual QSize minimumSizeHint(void) const;

    void setGradientFollowsValue(bool);

    void setGradientStartColor(QColor);
    void setGradientEndColor(QColor);

    void setSlider1Value(qreal);
    void setSlider2Value(qreal);
    void setValue(SliderNumber, qreal);

    qreal slider1Value(void) {return m_slider1Value;}
    qreal slider2Value(void) {return m_slider2Value;}
    qreal value(SliderNumber which=SLIDER1)
        {switch(which) {
            case SLIDER1:
                return m_slider1Value;
            case SLIDER2:
                return m_maximum - m_minimum - m_slider2Value;};
        }

protected slots:
    virtual void initStyleOption(QStyleOptionFrameV2*) const;
    virtual void initStyleOption(QStyleOptionSlider*) const;
    virtual void initStyleOptionSlider1(QStyleOptionSlider*) const;
    virtual void initStyleOptionSlider2(QStyleOptionSlider*) const;
    virtual void enterEvent(QEvent*);
    virtual void leaveEvent(QEvent*);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void paintEvent(QPaintEvent*);

private:
    int pixelValue(int position) const;

    bool m_hovering;
    bool m_pressed1;
    bool m_pressed2;
    bool m_dualSliders;
    bool m_slider1OnTop;
    bool m_valueGradient;

    int m_interval;
    int m_maximum;
    int m_minimum;
    qreal m_increment;
    int m_spacing;
    int m_pageSize;
    int m_slider1Position;
    qreal m_slider1Value;
    int m_slider2Position;
    qreal m_slider2Value;
    int m_offset;

    QRect m_handleRect;
    QRect m_guageRect;

    QColor m_gradStartColor;
    QColor m_gradEndColor;

    int m_lastX;
    GradientDirection m_direction;

signals:
    void rangeChanged(int, int);
    void sliderMoved(SliderNumber);
    void sliderPressed(SliderNumber);
    void sliderReleased(SliderNumber);
    void valueChanged(SliderNumber, int);
};

#endif
