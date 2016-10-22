/* */

#include "KioskSlideWidget.h"

KioskSlideWidget::KioskSlideWidget(QWidget *parent)
    : QWidget(parent),
    m_hovering(false),
    m_pressed1(false),
    m_pressed2(false),
    m_dualSliders(false),
    m_slider1OnTop(false),
    m_valueGradient(false),
    m_interval(1),
    m_maximum(10),
    m_minimum(0),
    m_increment(1.0),
    m_spacing(1),
    m_pageSize(2),
    m_slider1Position(0),
    m_slider1Value(0.0),
    m_slider2Position(0),
    m_slider2Value(0.0),
    m_offset(0),
    m_gradStartColor(QColor(122, 64, 52, 255)),
    m_gradEndColor(QColor(255, 255, 0, 255)),
    m_direction(DarkToLight)
{
    QWidget::setMouseTracking(true);
    QWidget::setFixedHeight(80);
    m_handleRect = QWidget::rect();
    m_lastX = 0;
}

void KioskSlideWidget::setRange(int min, int max, qreal increment, int spacing)
{
    m_minimum   = min;
    m_maximum   = max;

    if (increment <= 0)
        increment = 1;

    m_increment = increment;

    if (spacing <= 0)
        spacing = 1;

    m_spacing = spacing;

    repaint();

    emit rangeChanged(min, max);
}

void KioskSlideWidget::setSlider1Value(qreal value)
{
    m_slider1Position = value / m_increment;
    m_slider1Value = value;
    repaint();
}

void KioskSlideWidget::setSlider2Value(qreal value)
{
    m_slider2Value = value;
    value = m_maximum - m_minimum - value;
    m_slider2Position = value / m_increment;
    repaint();
}

void KioskSlideWidget::setValue(SliderNumber slider, qreal value)
{
    switch (slider) {
        case SLIDER1:
            setSlider1Value(value);
            break;
        case SLIDER2:
            setSlider2Value(value);
            break;
    };
}

void KioskSlideWidget::setDualSliders(bool set)
{
    m_dualSliders = set;
    repaint();
}

void KioskSlideWidget::setGradientFollowsValue(bool set)
{
    m_valueGradient = set;
    repaint();
}

void KioskSlideWidget::setGradientStartColor(QColor start)
{
    m_gradStartColor = start;
    repaint();
}

void KioskSlideWidget::setGradientEndColor(QColor end)
{
    m_gradEndColor = end;
    repaint();
}

void KioskSlideWidget::setDirection(GradientDirection direction)
{
    m_direction = direction;
    repaint();
}

QSize KioskSlideWidget::sizeHint(void) const
{
    return QSize(50, 10);
}

QSize KioskSlideWidget::minimumSizeHint(void) const
{
    return QSize(10, 4);
}

void KioskSlideWidget::initStyleOption(QStyleOptionFrameV2 *option) const
{
    option->initFrom(this);
    option->rect = m_guageRect;
    option->lineWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, option, this);
    option->midLineWidth = 0;
    option->state |= QStyle::State_Sunken;
    option->features = QStyleOptionFrameV2::None;
}

void KioskSlideWidget::initStyleOption(QStyleOptionSlider *option) const
{
    if (!option)
        return;

    option->initFrom(this);
    option->rect = m_handleRect;
    option->subControls = QStyle::SC_None|QStyle::SC_SliderHandle;
    option->activeSubControls = QStyle::SC_None;
    option->orientation = Qt::Horizontal;
    option->maximum = m_maximum / m_increment;
    option->minimum = m_minimum / m_increment;
    option->tickPosition = QSlider::TicksBelow;
    option->tickInterval = m_interval;
    option->singleStep = true;
    option->upsideDown = false;
    option->pageStep = m_pageSize;
    option->state |= QStyle::State_Horizontal;
}

void KioskSlideWidget::initStyleOptionSlider1(QStyleOptionSlider *option) const
{
    if (!option)
        return;

    initStyleOption(option);

    option->direction = Qt::LeftToRight;
    option->sliderPosition = m_slider1Position;
    option->sliderValue = m_slider1Value;
}

void KioskSlideWidget::initStyleOptionSlider2(QStyleOptionSlider *option) const
{
    if (!option)
        return;

    initStyleOption(option);

    option->direction = Qt::RightToLeft;
    option->sliderPosition = m_slider2Position;
    option->sliderValue = m_slider2Value;
}

void KioskSlideWidget::enterEvent(QEvent *event)
{
    m_hovering = true;
    QWidget::enterEvent(event);
}

void KioskSlideWidget::leaveEvent(QEvent *event)
{
    m_hovering = false;
    QWidget::leaveEvent(event);
}

void KioskSlideWidget::mousePressEvent(QMouseEvent *event)
{
    QStyleOptionSlider slider1Opt;
    QStyleOptionSlider slider2Opt;
    initStyleOptionSlider1(&slider1Opt);

    QRect rect1 = QWidget::style()->subControlRect(QStyle::CC_Slider, &slider1Opt, QStyle::SC_SliderHandle, this);
    QRect rect2;

    if (m_dualSliders) {
        initStyleOptionSlider2(&slider2Opt);
        rect2 = QWidget::style()->subControlRect(QStyle::CC_Slider, &slider2Opt, QStyle::SC_SliderHandle, this);
    }

    if (rect1.contains(event->pos())) {
        m_pressed1 = true;
        m_slider1OnTop = true;
        m_offset = event->pos().x() - rect1.topLeft().x();
        emit sliderPressed(SLIDER1);
    } else if (m_dualSliders && rect2.contains(event->pos())) {
        m_pressed2 = true;
        m_slider1OnTop = false;
        m_offset = event->pos().x() - rect2.topLeft().x();
        emit sliderPressed(SLIDER2);
    }

    repaint();
}

void KioskSlideWidget::mouseReleaseEvent(QMouseEvent*)
{
    if (m_pressed1) {
        emit sliderReleased(SLIDER1);
        emit valueChanged(SLIDER1, m_slider1Value);
    } else if (m_pressed2) {
        emit sliderReleased(SLIDER2);
        emit valueChanged(SLIDER2, (m_maximum - m_minimum - m_slider2Value));
    }

    m_pressed1 = false;
    m_pressed2 = false;
}

void KioskSlideWidget::mouseMoveEvent(QMouseEvent *event)
{
    QStyleOptionSlider sliderOpt;
    initStyleOption(&sliderOpt);
    QRect rect = QWidget::style()->subControlRect(QStyle::CC_Slider, &sliderOpt,
                                                  QStyle::SC_SliderGroove, this);

    int value = pixelValue(event->pos().x() - m_offset);

#warning: this fails with dual sliders and negative minimum
    if (m_pressed1) {
        int slider2 = (m_maximum / m_increment) - m_slider2Position;
        if (m_dualSliders && (value >= slider2))
            value = slider2 - 1;

        m_slider1Position = value;
        repaint();

        if (m_hovering)
            m_slider1Value = m_slider1Position * m_increment;

        emit sliderMoved(SLIDER1);
    } else if (m_pressed2) {
        int slider2 = (m_maximum / m_increment) - value;
        if (value <= m_slider1Position)
            slider2 = (m_maximum / m_increment) - m_slider1Position - 1;

        m_slider2Position = slider2;
        repaint();

        if (m_hovering)
            m_slider2Value = m_slider2Position * m_increment;

        emit sliderMoved(SLIDER2);
    }
}

void KioskSlideWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect rect = QWidget::rect();

    m_handleRect = rect.adjusted(0, 0, 0, -30);
    m_guageRect = rect.adjusted(0, 45, 0, 0);

    QStyleOptionFrameV2 panel;
    initStyleOption(&panel);

    QWidget::style()->drawPrimitive(QStyle::PE_Frame, &panel, &painter, this);

    QStyleOptionSlider slider1Opt;
    QStyleOptionSlider slider2Opt;
    initStyleOptionSlider1(&slider1Opt);
    if (m_dualSliders)
        initStyleOptionSlider2(&slider2Opt);

    int _minimum = m_minimum / m_increment;
    int _maximum = m_maximum / m_increment;

    int handleWidth = QWidget::style()->pixelMetric(QStyle::PM_SliderLength, &slider1Opt);

    // gradient
    int gradSpan = 0;
    QRect gradRect;
    QLinearGradient gradient;
    if (m_direction != NoGradient) {
        if (m_valueGradient) {
            int pos1 = QWidget::style()->sliderPositionFromValue(_minimum, _maximum, m_slider1Position,
                                                                (slider1Opt.rect.width() - handleWidth)) + (handleWidth / 2);
            int pos2 = QWidget::style()->sliderPositionFromValue(_minimum, _maximum, (m_maximum - m_minimum - m_slider2Position),
                                                                (slider2Opt.rect.width() - handleWidth)) + (handleWidth / 2);

            gradSpan = qAbs(pos2 - pos1);

            gradRect = QRect(pos1, m_guageRect.y(), gradSpan, m_guageRect.height());
        } else {
            gradRect = m_guageRect;
            gradSpan = rect.width();
        }

        gradient = QLinearGradient(gradSpan, 0, 0, 0);

        switch(m_direction) {
            case NoGradient:
                break;
            case DarkToLight:
                gradient.setColorAt(1.0, m_gradStartColor);
                gradient.setColorAt(0.0, m_gradEndColor);
                break;
            case LightToDark:
                gradient.setColorAt(0.0, m_gradEndColor);
                gradient.setColorAt(1.0, m_gradStartColor);
                break;
        };

        gradient.setSpread(QGradient::PadSpread);
    }

    painter.fillRect(m_guageRect, Qt::gray);

    if (m_direction != NoGradient) {
        painter.setBrush(gradient);
        painter.drawRect(gradRect);
    }

    // end gradient

    int numY = QWidget::height() - 2;

    for (int x=_minimum; x<=_maximum; x++) {
        int _pos = qMin(x, _maximum);
        int posX = QWidget::style()->sliderPositionFromValue(_minimum, _maximum, _pos,
                                                             (slider1Opt.rect.width() - handleWidth)) + (handleWidth / 2);

        qreal l_v = x * m_increment;
        QString label = QString("%1").arg(l_v);
        int cwidth = painter.fontMetrics().width(label);
        int numX = posX - (cwidth / 2);
        int posY = rect.height() - m_guageRect.height() + 1;

        int lineLength = 0;

        if ((fmod(l_v, 1) == 0) && !((int)l_v % m_spacing)) {
            painter.drawText(numX, numY, label);
            lineLength = 6;
        } else if ((fmod(l_v, 1) == 0) && ((int)l_v % m_spacing))
            lineLength = 8;
        else
            lineLength = 12;

        painter.drawLine(posX, posY, posX, numY - painter.fontMetrics().xHeight() - lineLength);
    }

    QRect grooveRect((m_handleRect.left() + 5), m_handleRect.center().y() - 3, (m_handleRect.width() - 10), 5);
    QLine line((grooveRect.x() + 2), (m_handleRect.center().y() + 1),
               grooveRect.width(), (m_handleRect.center().y() + 1));

    painter.setBrush(QColor(229, 227, 225));
    painter.setPen(QColor(186, 182, 178));
    painter.drawRoundedRect(grooveRect, 3, 3);
    painter.setPen(Qt::white);
    painter.drawLine(line);

    if (m_slider1OnTop) {
        if (m_dualSliders)
            QWidget::style()->drawComplexControl(QStyle::CC_Slider, &slider2Opt, &painter, this);
        QWidget::style()->drawComplexControl(QStyle::CC_Slider, &slider1Opt, &painter, this);
    } else {
        QWidget::style()->drawComplexControl(QStyle::CC_Slider, &slider1Opt, &painter, this);
        if (m_dualSliders)
            QWidget::style()->drawComplexControl(QStyle::CC_Slider, &slider2Opt, &painter, this);
    }
}

int KioskSlideWidget::pixelValue(int position) const
{
    QStyleOptionSlider sliderOpt;

    if (m_pressed1)
        initStyleOptionSlider1(&sliderOpt);
    else if (m_pressed2)
        initStyleOptionSlider2(&sliderOpt);
    else
        return -1;

    QRect grooveRect = QWidget::style()->subControlRect(QStyle::CC_Slider, &sliderOpt, QStyle::SC_SliderGroove, this);
    QRect sliderRect = QWidget::style()->subControlRect(QStyle::CC_Slider, &sliderOpt, QStyle::SC_SliderHandle, this);

    int min = grooveRect.x();
    int max = grooveRect.right() - sliderRect.width() + 1;

    int pixelPos = QStyle::sliderValueFromPosition(m_minimum / m_increment, m_maximum / m_increment,
                                                   position - min, (max - min), false);

    return pixelPos;
}

