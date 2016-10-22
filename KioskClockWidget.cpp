/* */

#include "KioskClockWidget.h"

#define PI 3.14159265

KioskClockWidget::KioskClockWidget(QWidget *parent)
    : QWidget(parent),
    m_clockFace(QPixmap(":/res/clock_face.png")),
    m_animation(new QPropertyAnimation(this, "rotation")),
    m_timer(new QTimer()),
    m_enabled(true)
{
    QWidget::setFixedSize(300, 300);

    m_animation->setDuration(100);
    m_timer->setInterval(1000);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateClock()));

    m_timer->start();
}

void KioskClockWidget::setShaded(QHash<int,int> shades)
{
    m_shades = shades;
    repaint();
}

void KioskClockWidget::updateClock(void)
{
    QTime time = QDateTime::currentDateTime().time();

    qreal target = 6.0 * time.second();
    target = (target == 0.0) ? 360.0 : target;
    m_angle = (m_angle == 360.0) ? 0.0 : m_angle;

    m_animation->setKeyValueAt(0.0, m_angle);
    m_animation->setKeyValueAt(1.0, target);
    m_animation->start();
}

void KioskClockWidget::setRotation(qreal angle)
{
    m_angle = angle;
    repaint();
}

void KioskClockWidget::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);

    if (event->type() ==  QEvent::EnabledChange)
        m_enabled = QWidget::isEnabled();
}

void KioskClockWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect rect = QWidget::rect();
    painter.drawPixmap(rect, m_clockFace);

    if (!m_enabled)
        return;

    int x, y;
    qreal shadowAngle;
    QTime time = QDateTime::currentDateTime().time();

    QRect hourHand(-2, 0, 4, -75);
    QRect minuteHand(-2, 0, 4, -90);
    QRect secondHand(-1, 0, 2, -90);
    QRect secondTail(-2, 0, 4, 30);

    shadowAngle = (15.0 * (time.minute() + time.second() / 60.0)) - 45;
    QRect hourShadow(-2+sin(shadowAngle*PI/180)*4.0, 0, 4, -73);

    shadowAngle = (6.0 * (time.minute() + time.second() / 60.0)) - 45;
    QRect minuteShadow(-2+sin(shadowAngle*PI/180)*4.5, 0, 4, -88);

    shadowAngle = m_angle - 45;
    QRect secondShadow(-1+sin(shadowAngle*PI/180)*4.0, 0, 2, -88);
    QRect secondTailShadow(-2+sin(shadowAngle*PI/180)*5.0, 0, 4, 28);

    painter.translate(148, 150);

    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient gradient(4, 0, 0, 0);
    gradient.setSpread(QGradient::PadSpread);

    gradient.setColorAt(0.0, QColor(180, 0, 0, 255));
    gradient.setColorAt(0.5, QColor(203, 63, 63, 255));
    gradient.setColorAt(1.0, QColor(180, 0, 0, 255));

    QRadialGradient gradient2(3, 3, 18, 3, 3);

    gradient2.setColorAt(0.0, QColor(255, 120, 120, 255));
    gradient2.setColorAt(1.0, QColor(180, 0, 0, 255));

    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.rotate(15.0 * ((time.hour() + time.minute() / 60.0)));
    painter.setOpacity(0.1);
    painter.drawRoundedRect(hourShadow, 4, 4);
    painter.setOpacity(1.0);
    painter.drawRoundedRect(hourHand, 4, 4);
    painter.restore();

    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.setOpacity(0.1);
    painter.drawRoundedRect(minuteShadow, 4, 4);
    painter.setOpacity(1.0);
    painter.drawRoundedRect(minuteHand, 4, 4);
    painter.restore();

    painter.save();
    painter.setPen(Qt::NoPen);
    painter.rotate(m_angle);
    painter.setOpacity(0.1);
    painter.setBrush(Qt::black);
    painter.drawRoundedRect(secondShadow, 1, 1);
    painter.drawRoundedRect(secondTailShadow, 2, 2);
    painter.setOpacity(1.0);
    painter.setBrush(gradient);
    painter.drawRoundedRect(secondHand, 1, 1);
    painter.drawRoundedRect(secondTail, 2, 2);
    painter.restore();

    QRect centerRect(-3, -3, 6, 6);
    painter.setBrush(gradient2);
    painter.drawEllipse(centerRect);

    painter.translate(-148, -150);
    painter.setOpacity(0.3);

    rect.adjust(25, 25, -25, -25);

    QHashIterator<int,int> i(m_shades);
    while (i.hasNext()) {
        i.next();
        qreal start = i.key();
        qreal end = i.value();

        painter.setBrush(Qt::red);

        qreal seconds = end - start;
        start = (start / 86400) * 360 * 16;
        end = (seconds / 86400) * 360 * 16;

        painter.drawPie(rect, -start+1440, -end);
    }
}
