/* */

#include "KioskButtonBase.h"

// From QAbstractButton
#define AUTO_REPEAT_DELAY  300
#define AUTO_REPEAT_INTERVAL 100

KioskButtonBase::KioskButtonBase(QWidget *parent)
    : QWidget(parent)
{
    QWidget::setMouseTracking(true);

    is_checkable  = false;
    is_checked    = false;
    is_pressed    = false;
    is_enabled    = true;
    is_mutual     = false;
    is_autorepeat = false;
    is_hovering   = false;
    did_hover     = false;

    button_opacity = 1.0;

    button_state = ButtonStateReleased;
    button_type  = ButtonTypeNormal;

    autorepeat_delay = AUTO_REPEAT_DELAY;
    autorepeat_interval = AUTO_REPEAT_INTERVAL;

    autorepeat_timer = new QTimer();
    autorepeat_timer->setSingleShot(true);

    _setup();

    connect(autorepeat_timer, SIGNAL(timeout()), this, SLOT(slot_check_autorepeat()));
}

void KioskButtonBase::toggle(void)
{
    if (is_checkable)
        setChecked(!is_checked);
}

void KioskButtonBase::setButtonType(KioskKeyboardButtonType type)
{
    if (type != button_type) {
        button_type = type;
        repaint();
    }
}

void KioskButtonBase::setChecked(bool checked, bool emitter)
{
    if (is_checkable) {
        is_checked = checked;

        if (checked)
            button_state = ButtonStateChecked;
        else
            button_state = ButtonStateReleased;

        repaint();

        if (emitter)
            emit toggled();
    }

    if (emitter)
        emit clicked();
}

void KioskButtonBase::setMutual(bool mutual)
{
    is_mutual = mutual;
}

void KioskButtonBase::setOpacity(float opacity)
{
    button_opacity = opacity;
    repaint();
}

void KioskButtonBase::_setup(void)
{
    gradients[ButtonTypeNormal][ButtonStateReleased]  =
        new ButtonGradColor(QColor(0, 0, 0, 223), QColor(93, 110, 123, 251));
    gradients[ButtonTypeNormal][ButtonStatePressed]   =
        new ButtonGradColor(QColor(0, 0, 0, 248), QColor(54, 64,   72, 251));
    gradients[ButtonTypeNormal][ButtonStateChecked]   =
        new ButtonGradColor(QColor(50, 10, 10, 251), QColor(255, 100, 100, 248));
    gradients[ButtonTypeNormal][ButtonStateUnchecked] =
        gradients[ButtonTypeNormal][ButtonStateReleased];
    gradients[ButtonTypeNormal][ButtonStateDisabled] =
        new ButtonGradColor(QColor(0, 0, 0, 172), QColor(22, 26, 29, 232));

    gradients[ButtonTypeOkClose][ButtonStateReleased]  =
        new ButtonGradColor(QColor(0, 0, 0, 248), QColor(94, 159, 209, 251));
    gradients[ButtonTypeOkClose][ButtonStatePressed]   =
        gradients[ButtonTypeNormal][ButtonStatePressed];
    gradients[ButtonTypeOkClose][ButtonStateChecked]   =
        gradients[ButtonTypeNormal][ButtonStateChecked];
    gradients[ButtonTypeOkClose][ButtonStateUnchecked] =
        gradients[ButtonTypeOkClose][ButtonStateReleased];
    gradients[ButtonTypeOkClose][ButtonStateDisabled] =
        gradients[ButtonTypeNormal][ButtonStateDisabled];
}

void KioskButtonBase::enterEvent(QEvent *event)
{
    if (button_state != ButtonStateDisabled) {
        is_hovering = true;
        repaint();
    }

    QWidget::enterEvent(event);
}

void KioskButtonBase::leaveEvent(QEvent *event)
{
    if (button_state != ButtonStateDisabled) {
        is_hovering = false;
        repaint();
    }

    QWidget::leaveEvent(event);
}

void KioskButtonBase::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);

    if (event->type() ==  QEvent::EnabledChange) {
        is_enabled = QWidget::isEnabled();

        if (is_checkable && is_checked)
            button_state = is_enabled ? ButtonStateChecked : ButtonStateDisabled;
        else
            button_state = is_enabled ? ButtonStateReleased : ButtonStateDisabled;

        repaint();
    }
}

void KioskButtonBase::mousePressEvent(QMouseEvent *event)
{
    if (!is_enabled)
        return;

    if (is_mutual && is_checked)
        return;
    is_pressed = true;
    button_state = ButtonStatePressed;
    repaint();
    emit pressed();

    if (is_autorepeat) {
        autorepeat_timer->start(autorepeat_delay);
    }
}

void KioskButtonBase::mouseReleaseEvent(QMouseEvent *event)
{
    if (!is_enabled)
        return;

    if (!QWidget::rect().contains(event->pos())) {
        if (is_checkable)
            button_state = is_checked ? ButtonStateChecked : ButtonStateUnchecked;
        else
            button_state = ButtonStateReleased;
        emit released();
        repaint();
        return;
    }

    if (is_checkable) {
        if (is_mutual && is_checked)
            return;

        is_checked = !is_checked;
        button_state = is_checked ? ButtonStateChecked : ButtonStateUnchecked;
    } else
        button_state = ButtonStateReleased;

    is_pressed = false;

    repaint();
    emit clicked();

    if (is_checkable)
        emit toggled();

    emit released();
}

void KioskButtonBase::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setOpacity(button_opacity);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    if (button_opacity > 0.0) {
        QRect rect = QWidget::rect(); // we want to redraw the entire widget

        ButtonGradColor *grads = gradients[button_type][button_state];

        QColor start = grads->start();
        QColor end = grads->end();

        if (is_hovering) {
            start = start.lighter(125);
            end = end.lighter(125);
        }

        QLinearGradient gradient(0, 100, 0, 0);

        if (is_checkable && is_checked) {
            gradient.setColorAt(1.0, start);
            gradient.setColorAt(0.0, end);
        } else {
            gradient.setColorAt(0.0, start);
            gradient.setColorAt(1.0, end);
        }

        gradient.setSpread(QGradient::PadSpread);

        rect.adjust(0, 0, -1, -1); // prevent bottom and right from getting cut
        QPen pen;
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(1);
        pen.setBrush(Qt::black);

        painter.setPen(pen);

        QPainterPath path;
        path.addRoundedRect(rect, 14, 14);
        painter.setBrush(gradient);
        painter.drawPath(path);
    } else
        painter.fillRect(event->rect(), Qt::black);
}

void KioskButtonBase::slot_check_autorepeat(void)
{
    if (is_autorepeat && is_pressed) {
        emit pressed();
        autorepeat_timer->start(autorepeat_interval);
    }
}
