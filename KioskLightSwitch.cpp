/* */

#include "KioskLightSwitch.h"

KioskLightSwitch::KioskLightSwitch(QWidget *parent)
    : KioskButtonBase(parent),
    m_background(QPixmap(":/res/switch_background.png")),
    m_switchOn(QPixmap(":/res/switch_on.png")),
    m_switchOff(QPixmap(":/res/switch_off.png")),
    m_checked(false)
{
    KioskButtonBase::setCheckable(true);
    KioskButtonBase::setOpacity(0.0);
    KioskButtonBase::setFixedSize(QSize(66, 127));
}

void KioskLightSwitch::mouseReleaseEvent(QMouseEvent *event)
{
    KioskButtonBase::mouseReleaseEvent(event);

    bool needs_repaint = false;

    if (m_checked != KioskButtonBase::isChecked())
        needs_repaint = true;

    m_checked = KioskButtonBase::isChecked();

    if (needs_repaint)
        repaint();
}

void KioskLightSwitch::setChecked(bool checked, bool emitter)
{
    KioskButtonBase::setChecked(checked, emitter);

    bool needs_repaint = false;

    if (m_checked != checked)
        needs_repaint = true;

    m_checked = checked;

    if (needs_repaint)
        repaint();
}

void KioskLightSwitch::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.drawPixmap(QWidget::rect(), m_background);

    if (m_checked)
        painter.drawPixmap(QWidget::rect(), m_switchOn);
    else
        painter.drawPixmap(QWidget::rect(), m_switchOff);
}

KioskLightDimmer::KioskLightDimmer(QWidget *parent)
    : KioskButtonBase(parent),
    m_background(QPixmap(":/res/dimmer_background.png")),
    m_button(QPixmap(":/res/dimmer_button.png")),
    m_slider(QPixmap(":/res/dimmer_slider.png")),
    m_blueled(QPixmap(":/res/dimmer_blueled.png")),
    m_greenled(QPixmap(":/res/dimmer_greenled.png")),
    m_animateTimer(new QTimer()),
    m_animateDir(0),
    m_dimmer(0),
    m_oldDimmer(0),
    m_positionY(67),
    m_oldPositionY(0),
    m_buttonPressed(false),
    m_sliderPressed(false),
    m_animating(false)
{
    KioskButtonBase::setOpacity(0.0);
    KioskButtonBase::setFixedSize(82, 169);
}

void KioskLightDimmer::setDimmer(int value)
{
    m_oldDimmer = m_dimmer = value;
    if (!m_animating) {
        m_positionY = 67 - (m_dimmer / 2);
        repaint();
    }
}

int KioskLightDimmer::dimmer(void)
{
    return m_dimmer;
}

void KioskLightDimmer::mousePressEvent(QMouseEvent *event)
{
    KioskButtonBase::mousePressEvent(event);

    bool needs_repaint = false;

    QRect button_rect(17, 107, 53, 45);
    if (button_rect.contains(event->pos())) {
        m_buttonPressed = true;
        m_sliderPressed = false;
        needs_repaint = true;
    } else {
        QRect slider_rect(14, m_positionY, 59, 34);
        if (slider_rect.contains(event->pos())) {
            m_sliderPressed = true;
            m_buttonPressed = false;
            m_oldPositionY = m_positionY;
            needs_repaint = true;
        }
    }

    m_pressPos = event->pos();

    if (needs_repaint)
        repaint();
}

void KioskLightDimmer::mouseReleaseEvent(QMouseEvent *event)
{
    KioskButtonBase::mouseReleaseEvent(event);

    bool needs_repaint = false;
    bool needs_animate = false;

    if (m_buttonPressed) {
        QRect button_rect(17, 107, 53, 45);
        if (button_rect.contains(event->pos())) {
            m_dimmer = (m_dimmer > 0) ? 0 : 100;
            emit sliderChanged(m_dimmer);
            emit sliderMoved(m_dimmer);
            m_animateDir = m_dimmer ? -2 : 2;
            m_positionY += m_animateDir;
            needs_animate = true;
        }

    } else if (m_sliderPressed) {
        // only check x coords
        if ((event->pos().x() >= 14) && (event->pos().x() <= 73)) {
            m_dimmer = (67 - m_positionY) * 2;
            if (m_dimmer != m_oldDimmer) {
                emit sliderChanged(m_dimmer);
                m_oldDimmer = m_dimmer;
            }
            needs_repaint = true;
        } else {
            m_positionY = m_oldPositionY;
            m_dimmer = m_oldDimmer;
            emit sliderMoved(m_dimmer);
            needs_repaint = true;
        }
    }

    m_buttonPressed = false;
    m_sliderPressed = false;

    if (needs_repaint)
        repaint();

    if (needs_animate)
        animateSlider();
}

void KioskLightDimmer::mouseMoveEvent(QMouseEvent *event)
{
    QRect slider_rect(14, m_positionY, 59, 34);
    if (slider_rect.contains(event->pos())) {
        m_movePos = event->pos();

        int diff = 0;
        if (m_sliderPressed) {
            diff = m_pressPos.y() - m_movePos.y();
            m_positionY -= diff;
            m_positionY  = (m_positionY < 17) ? 17 : m_positionY;
            m_positionY  = (m_positionY > 67) ? 67 : m_positionY;
            m_pressPos   = m_movePos;
            m_dimmer = (67 - m_positionY) * 2;
            emit sliderMoved(m_dimmer);
            repaint();
        }
    }
}

void KioskLightDimmer::animateSlider(void)
{
    if ((m_positionY > 17) && (m_positionY < 67) &&
        !m_buttonPressed && !m_sliderPressed) {
        m_animating = true;
        m_positionY += m_animateDir;
        m_positionY  = (m_positionY < 17) ? 17 : m_positionY;
        m_positionY  = (m_positionY > 67) ? 67 : m_positionY;
        repaint();
        m_animateTimer->singleShot(10, this, SLOT(animateSlider()));
    } else
        m_animating = false;
}

void KioskLightDimmer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.drawPixmap(QWidget::rect(), m_background);
    painter.drawPixmap(14, m_positionY, m_slider);

    int ledX = 29;
    int ledY = 113;

    if (!m_buttonPressed) {
        painter.drawPixmap(17, 107, m_button);
        ledX += 2;
        ledY += 2;
    }

    if (m_dimmer > 0)
        painter.drawPixmap(ledX, ledY, m_greenled);
    else
        painter.drawPixmap(ledX, ledY, m_blueled);
}
