/* */

#include "KioskButton.h"

KioskButton::KioskButton(QWidget *parent)
    : KioskButtonBase(parent)
{
    text_label = 0;
    icon_label = 0;

    enabled_color = QColor("#eeeeee");
    disabled_color = QColor("#888888");

    text_font.setPointSize(16);
    text_font.setBold(true);

    layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    layout->setMargin(10);
    layout->setSpacing(5);
    layout->setSizeConstraint(QLayout::SetMinimumSize);
}

void KioskButton::setMargin(int margin)
{
    layout->setMargin(margin);
}

int KioskButton::margin(void)
{
    return layout->margin();
}

void KioskButton::setText(QString text)
{
    if (!text_label) {
        text_label = new QLabel();
        QPalette palette = text_label->palette();
        palette.setColor(QPalette::Foreground, enabled_color);
        text_label->setPalette(palette);
        text_label->setFont(text_font);
        layout->addWidget(text_label, 0, Qt::AlignHCenter|Qt::AlignVCenter);
    }

    text_label->setText(text);
    button_text = text;
}

void KioskButton::setIcon(QIcon icon)
{
    if (!icon_label) {
        icon_label = new QLabel();
        layout->insertWidget(0, icon_label, 0, Qt::AlignHCenter|Qt::AlignVCenter);
    }

    QPixmap pixmap = _getPixmap(icon, icon_size);

    icon_label->setPixmap(pixmap);
    button_icon = icon;
}

void KioskButton::setWidget(QWidget *_widget)
{
    if (icon_label) {
        layout->removeWidget(icon_label);
        icon_label->deleteLater();
        icon_label = 0;
    }

    layout->insertWidget(0, _widget, 0, Qt::AlignHCenter|Qt::AlignVCenter);
    m_widget = _widget;
}

void KioskButton::setFont(QFont font)
{
    text_font = font;

    if (text_label)
        text_label->setFont(font);
}

void KioskButton::setEnabled(bool enabled)
{
    if (text_label) {
        QPalette palette = text_label->palette();

        if (enabled)
            palette.setColor(QPalette::Foreground, enabled_color);
        else
            palette.setColor(QPalette::Foreground, disabled_color);

        text_label->setPalette(palette);
    }

    KioskButtonBase::setEnabled(enabled);

    if (!button_icon.isNull())
        setIcon(button_icon);
}

void KioskButton::setIconSize(QSize size)
{
    icon_size = size;
    setIcon(button_icon);
}

void KioskButton::setButtonStyle(KioskButtonStyle style) {
    button_style = style;

    switch (button_style) {
        case IconOnly:
            if (text_label) {
                text_label->hide();
                text_label->deleteLater();
            }
            text_label = 0;
            break;
        case TextOnly:
            if (icon_label) {
                icon_label->hide();
                icon_label->deleteLater();
            }
            icon_label = 0;
            break;
        case TextBesideIcon:
            layout->setDirection(QBoxLayout::LeftToRight);
            break;
        case TextUnderIcon:
            layout->setDirection(QBoxLayout::TopToBottom);
            break;
    }
}

QPixmap KioskButton::_getPixmap(QIcon icon, QSize size)
{
    QPixmap pixmap;

    if (size.isNull()) {
        QList<QSize> available = icon.availableSizes();
        size = available[0];
    }

    if (KioskButtonBase::isEnabled()) {
        if (KioskButtonBase::isPressed())
            pixmap = icon.pixmap(size, QIcon::Active, QIcon::On);
        else
            pixmap = icon.pixmap(size, QIcon::Normal, QIcon::On);
    } else
        pixmap = icon.pixmap(size, QIcon::Disabled, QIcon::Off);

    return pixmap;
}

void KioskButton::paintEvent(QPaintEvent *event)
{
    KioskButtonBase::setFixedSize(layout->sizeHint());
    KioskButtonBase::paintEvent(event);
}
