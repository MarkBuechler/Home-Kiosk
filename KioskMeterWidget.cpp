/* */

#include "KioskMeterWidget.h"

KioskMeterWidget::KioskMeterWidget(QWidget *parent)
    : QWidget(parent),
    m_value(0.0),
    m_angle(0.0),
    m_rangeLow(-180.0),
    m_rangeHigh(180.0),
    m_animation(new QPropertyAnimation(this, "rotation"))
{
    m_curve.setType(QEasingCurve::OutElastic);
    m_curve.setPeriod(0.45);
    m_curve.setAmplitude(1.0);
    m_animation->setEasingCurve(m_curve);

    QWidget::setMouseTracking(true);
}

QSize KioskMeterWidget::sizeHint(void)
{
    return QWidget::size();
}

void KioskMeterWidget::setNeedlePixmaps(QPixmap needle, QPixmap shadow)
{
    m_needle = needle;
    m_shadow = shadow;
    repaint();
}

void KioskMeterWidget::setBackgroundPixmap(QPixmap background)
{
    m_background = background;
    repaint();
}

void KioskMeterWidget::setOverlayPixmap(QPixmap overlay)
{
    m_overlay = overlay;
    repaint();
}

void KioskMeterWidget::setLedPixmap(QPixmap led)
{
    m_led = led;
    repaint();
}

void KioskMeterWidget::setNeedleOffset(QPoint offset)
{
    m_needleOffset = offset;
    repaint();
}

void KioskMeterWidget::setNeedleOrigins(QPoint needle, QPoint shadow)
{
    m_needleOrigin = needle;
    m_shadowOrigin = shadow;
    repaint();
}

void KioskMeterWidget::setNeedleShadowOffset(QPoint offset)
{
    m_shadowOffset = offset;
    repaint();
}

void KioskMeterWidget::setOverlayOffset(QPoint offset)
{
    m_overlayOffset = offset;
    repaint();
}

void KioskMeterWidget::setLedOffset(QPoint offset)
{
    m_ledOffset = offset;
    repaint();
}

void KioskMeterWidget::setLabelRect(QRectF rect)
{
    m_labelRect = rect;
    repaint();
}

void KioskMeterWidget::setMeterRange(qreal low, qreal high)
{
    m_rangeLow = low;
    m_rangeHigh = high;
}

void KioskMeterWidget::setValue(qreal value)
{
    if (m_value == value)
        return;

    m_value = value;

    int duration = ((qAbs(m_angle - m_angle) / 180) * 2000) + 1000;

    m_animation->stop();
    m_animation->setKeyValueAt(0.0, m_angle);
    m_animation->setKeyValueAt(1.0, m_value);
    m_animation->setDuration(duration);
    m_animation->start();
}

void KioskMeterWidget::setLabel(QString label)
{
    m_label = label;
    repaint();
}

void KioskMeterWidget::setRotation(qreal angle)
{
    m_angle = angle;

    m_angle = (m_angle < m_rangeLow) ? m_rangeLow : m_angle;
    m_angle = (m_angle > m_rangeHigh) ? m_rangeHigh : m_angle;

    repaint();
}

void KioskMeterWidget::mousePressEvent(QMouseEvent*)
{
    m_animation->stop();
    m_animation->setKeyValueAt(0.0,  m_angle);
    m_animation->setKeyValueAt(0.25, m_angle + 5);
    m_animation->setKeyValueAt(0.75, m_angle - 5);
    m_animation->setKeyValueAt(1.0,  m_value);
    m_animation->setDuration(qAbs(m_animation->keyValueAt(0.25).toReal() -
                                  m_animation->keyValueAt(0.75).toReal()) * 200.0);
    m_animation->start();
}

void KioskMeterWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    QTransform transform;
    transform.translate(m_needleOrigin.x(), m_needleOrigin.y());
    transform.rotate(m_angle);
    QPixmap rotated = m_needle.transformed(transform, Qt::SmoothTransformation);
    QTransform needleTransformed = QPixmap::trueMatrix(transform, m_needle.width(), m_needle.height());

    transform.reset();
    transform.translate(m_shadowOrigin.x(), m_shadowOrigin.y());
    transform.rotate(m_angle);
    QPixmap shadow = m_shadow.transformed(transform, Qt::SmoothTransformation);
    QTransform shadowTransformed = QPixmap::trueMatrix(transform, m_shadow.width(), m_shadow.height());

    painter.drawPixmap(0, 0, m_background);

    painter.translate(m_needleOffset);

    int posX = (m_angle < 0) ? ((m_needleOffset.x() + 1) - rotated.width())
        : (m_needleOffset.x() - 1);
    int posY = m_needleOffset.y() - rotated.height();

    int x, y;

    shadowTransformed.map(m_needleOrigin.x(), m_needleOrigin.y(), &x, &y);
    painter.drawPixmap(-x+m_shadowOffset.x(), -y+m_shadowOffset.y(), shadow);

    needleTransformed.map(m_needleOrigin.x(), m_needleOrigin.y(), &x, &y);
    painter.drawPixmap(-x, -y, rotated);

    painter.translate(-m_needleOffset.x(), -m_needleOffset.y());

    painter.drawPixmap(m_overlayOffset.x(), m_overlayOffset.y(), m_overlay);
    painter.drawPixmap(m_ledOffset.x(), m_ledOffset.y(), m_led);
    painter.drawText(m_labelRect, Qt::AlignHCenter, m_label);
}

qreal value_to_percent(qreal, MeterRange, int);

KioskWattMeter::KioskWattMeter(QWidget *parent)
    : KioskMeterWidget(parent),
    m_currentRange(WATT_AUTO),
    m_background(QPixmap(":/res/watt_meter.png")),
    m_overlay(QPixmap(":/res/meter_overlay.png")),
    m_needle(QPixmap(":/res/needle_watt.png")),
    m_shadow(QPixmap(":/res/needle_watt_shadow.png")),
    m_dled(QPixmap(":/res/dark_led.png")),
    m_lled(QPixmap(":/res/light_led.png")),
    m_currentPercent(0.0)
{
    QWidget::setFixedSize(306, 172);

    KioskMeterWidget::setNeedlePixmaps(m_needle, m_shadow);
    KioskMeterWidget::setBackgroundPixmap(m_background);
    KioskMeterWidget::setOverlayPixmap(m_overlay);
    KioskMeterWidget::setLedPixmap(m_dled);
    KioskMeterWidget::setNeedleOffset(QPoint(150, 185));
    KioskMeterWidget::setNeedleShadowOffset(QPoint(-10, 10));
    KioskMeterWidget::setLedOffset(QPoint(256, 135));
    KioskMeterWidget::setOverlayOffset(QPoint(0, 0));
    KioskMeterWidget::setLabelRect(QRectF(0, 140, KioskMeterWidget::width(),
                                         (KioskMeterWidget::height() - 140)));
    KioskMeterWidget::setNeedleOrigins(QPoint(2, m_needle.height()),
                                       QPoint(4, m_shadow.height()));
    KioskMeterWidget::setMeterRange(-60, 60);

    setMeterValue(0.0);
}

void KioskWattMeter::setRange(MeterRange range)
{
    if (range == WATT_AUTO) {
        if (m_currentPercent == 110) {
            range = WATT_RANGE2;
            KioskMeterWidget::setLedPixmap(m_lled);
        } else {
            range = WATT_RANGE1;
            KioskMeterWidget::setLedPixmap(m_dled);
        }
    }

    m_currentRange = range;
}

void KioskWattMeter::setMeterValue(qreal _value)
{
    bool found = false;

    if (m_currentRange == WATT_AUTO)
        setRange();

    if ((_value < 2000) && (m_currentRange == WATT_RANGE2)) {
        setRange();
    }

    qreal percent = value_to_percent(_value, m_currentRange, WATT_RANGE_SIZE);

    if ((percent < 0) && (m_currentPercent != 110.0) && (_value > 0)) {
        m_currentPercent = 110.0;
        found = true;
        setRange();
        percent = value_to_percent(_value, m_currentRange, WATT_RANGE_SIZE);
    } else
        found = true;

    if (found) {
        percent = floor(percent*10.0+0.5)/10.0;
        if (!m_currentPercent || (qAbs(m_currentPercent - percent) > 0.5))
            setValue(percent);
    }
}

void KioskWattMeter::setValue(qreal value)
{
    m_currentPercent = value;
    value = qMin(qMax(-50.0, value * 0.98 - 50.0), 53.0);
    KioskMeterWidget::setValue(value);
}

KioskVoltMeter::KioskVoltMeter(QWidget *parent)
    : KioskMeterWidget(parent),
    m_currentRange(VOLT_AUTO),
    m_background(QPixmap(":/res/volt_meter.png")),
    m_overlay(QPixmap(":/res/meter_overlay.png")),
    m_needle(QPixmap(":/res/needle_volt.png")),
    m_shadow(QPixmap(":/res/needle_volt_shadow.png")),
    m_currentPercent(0.0)
{
    QWidget::setFixedSize(306, 172);

    KioskMeterWidget::setNeedlePixmaps(m_needle, m_shadow);
    KioskMeterWidget::setBackgroundPixmap(m_background);
    KioskMeterWidget::setOverlayPixmap(m_overlay);
    KioskMeterWidget::setNeedleOffset(QPoint(150, 185));
    KioskMeterWidget::setNeedleShadowOffset(QPoint(-10, 0));
    KioskMeterWidget::setOverlayOffset(QPoint(0, 0));
    KioskMeterWidget::setLabelRect(QRectF(0, 140, KioskMeterWidget::width(),
                                         (KioskMeterWidget::height() - 140)));
    KioskMeterWidget::setNeedleOrigins(QPoint(2, m_needle.height()),
                                       QPoint(4, m_shadow.height()));
    KioskMeterWidget::setMeterRange(-60, 60);

    setMeterValue(0.0);
}

void KioskVoltMeter::setRange(MeterRange range)
{
    if (range == VOLT_AUTO) {
        range = VOLT_RANGE1;
    }

    m_currentRange = range;
}

void KioskVoltMeter::setMeterValue(qreal _value)
{
    bool found = false;

    if (m_currentRange == VOLT_AUTO)
        setRange();

    qreal percent = value_to_percent(_value, m_currentRange, VOLT_RANGE_SIZE);

    if ((percent < 0) && (m_currentPercent != 110.0) && (_value > 0)) {
        m_currentPercent = 110.0;
        found = true;
        setRange();
        percent = value_to_percent(_value, m_currentRange, VOLT_RANGE_SIZE);
    } else
        found = true;

    if (found) {
        percent = floor(percent*10.0+0.5)/10.0;
        if (!m_currentPercent || (qAbs(m_currentPercent - percent) > 0.5))
            setValue(percent);
    }
}

void KioskVoltMeter::setValue(qreal value)
{
    m_currentPercent = value;
    value = qMin(qMax(-45.0, value * 0.98 - 45.0), 48.0);
    KioskMeterWidget::setValue(value);
}

qreal value_to_percent(qreal _value, MeterRange range, int size)
{
    qreal percent;
    bool found=false;

    for(int x=1; x<size; x++) {
        qreal value, power, p_value, p_power;
        value = power = p_value = p_power = 0.0;
        switch(range) {
            case WATT_AUTO:
            case VOLT_AUTO:
                return -1;
            case WATT_RANGE1:
                value = watt_range1[x][0];
                power = watt_range1[x][1];
                p_value = watt_range1[x-1][0];
                p_power = watt_range1[x-1][1];
                break;
            case WATT_RANGE2:
                value = watt_range2[x][0];
                power = watt_range2[x][1];
                p_value = watt_range2[x-1][0];
                p_power = watt_range2[x-1][1];
                break;
            case VOLT_RANGE1:
                value = volt_range1[x][0];
                power = volt_range1[x][1];
                p_value = volt_range1[x-1][0];
                p_power = volt_range1[x-1][1];
                break;
        };

        if ((p_power <= _value) && (_value <= power)) {
            qreal diff = power - p_power;
            qreal perc = (_value - p_power) / diff;
            qreal diff2 = value - p_value;
            qreal add = perc * diff2;
            percent = p_value + add;
            found = true;
            break;
        }
    }

    if (found)
        return percent;

    return -1;
}
