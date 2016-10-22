/* */

#include "KioskLineEdit.h"

KioskLineEdit::KioskLineEdit(QWidget *parent)
    : QLineEdit(parent),
    m_progressWidth(0)
{
    QLineEdit::setFrame(false);
    QPalette palette = QLineEdit::palette();
    palette.setBrush(QPalette::Base, QBrush(Qt::transparent));
    QLineEdit::setPalette(palette);

}

void KioskLineEdit::setProgress(float progress)
{
    m_progressWidth = progress;
    repaint();
}

void KioskLineEdit::setIcon(QIcon _icon)
{
    m_icon = _icon;
    setMargins();
    repaint();
}

void KioskLineEdit::mouseMoveEvent(QMouseEvent *event)
{
    if (m_buttonRect.contains(event->pos()))
        QLineEdit::setCursor(Qt::ArrowCursor);
    else
        QLineEdit::setCursor(Qt::IBeamCursor);

    QLineEdit::mouseMoveEvent(event);
}

void KioskLineEdit::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_buttonRect.contains(event->pos())) {
        m_justCleared = true;
        QLineEdit::clear();
    }

    QLineEdit::mouseReleaseEvent(event);
}

void KioskLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    emit activated();
}

void KioskLineEdit::focusOutEvent(QFocusEvent *event)
{
    if (m_justCleared) {
        QLineEdit::undo();
        m_justCleared = "";
    }

    QLineEdit::focusOutEvent(event);
    emit deactivated();
}

bool KioskLineEdit::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
        m_justCleared = false;

    return QLineEdit::event(event);
}

void KioskLineEdit::resizeEvent(QResizeEvent *event)
{
    QLineEdit::resizeEvent(event);

    setMargins();
}

void KioskLineEdit::setMargins(void)
{
    QMargins margins = QLineEdit::textMargins();
    margins.setRight(24);
    if (!m_icon.isNull())
        margins.setLeft(24);
    QLineEdit::setTextMargins(margins);
}

void KioskLineEdit::initStyleOption(QStyleOptionFrameV2 *option) const
{
    // from qt demo browser
    option->initFrom(this);
    option->rect = contentsRect();
    option->lineWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, option, this);
    option->midLineWidth = 0;
    option->state |= QStyle::State_Sunken;
    if (QLineEdit::isReadOnly())
        option->state |= QStyle::State_ReadOnly;

    option->features = QStyleOptionFrameV2::None;
}

void KioskLineEdit::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QColor baseC = QLineEdit::palette().color(QPalette::Base);

    // from qt demo browser
    QStyleOptionFrameV2 panel;
    initStyleOption(&panel);
    QLineEdit::style()->drawPrimitive(QStyle::PE_PanelLineEdit, &panel, &painter, this);

    QRect brect = style()->subElementRect(QStyle::SE_LineEditContents, &panel, this);

    if (m_progressWidth && (m_progressWidth < 100)) {
        QLinearGradient gradient(0, 0, brect.width(), 0);

        QColor progC = QColor(0, 95, 255, 255);

        gradient.setColorAt(0.0, baseC);
        gradient.setColorAt(0.25, progC.lighter(100));
        gradient.setColorAt(0.50, progC);
        gradient.setColorAt(0.75, progC.lighter(100));
        gradient.setColorAt(1.0, baseC);
        gradient.setSpread(QGradient::PadSpread);

        int end = brect.width() * (m_progressWidth / 100);

        QRect bar = QRect(0, 2, end, brect.height());

        painter.setBrush(gradient);
        painter.setPen(Qt::transparent);
        painter.drawRect(bar);
    }

    QLineEdit::paintEvent(event);

    QRect rect = QLineEdit::rect();

    int width = rect.width();
    int height = rect.height();

    int leftX   = 4;
    int rightX  = width - 24;
    int centerY = (height / 2) - 8;

    if (!m_icon.isNull()) {
        QPoint ipoint(leftX, centerY);
        QPixmap ipixmap = m_icon.pixmap(16, 16);
        painter.drawPixmap(ipoint, ipixmap, rect);
    }

    QPoint cpoint(rightX, centerY + 2);

    m_buttonRect = QRect((rightX - 2), centerY, height, 20);

    QPixmap cpixmap(":/res/lineedit_clear.png");

    painter.drawPixmap(cpoint, cpixmap, rect);
}
