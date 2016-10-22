/* */

#include "KioskCheckbox.h"

KioskCheck::KioskCheck(QWidget *parent)
    : KioskButtonBase(parent),
    m_checkmark(QPixmap(":/res/sched_checkmark.png"))
{
    KioskButtonBase::setOpacity(0.0);
    KioskButtonBase::setCheckable(true);
    KioskButtonBase::setFixedSize(QStyle::PM_DefaultFrameWidth + 34, QStyle::PM_DefaultFrameWidth + 34);
}

void KioskCheck::initStyleOption(QStyleOptionFrameV2 *option) const
{
    option->initFrom(this);
    option->rect = contentsRect();
    option->lineWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, option, this);
    option->midLineWidth = 0;
    option->state |= QStyle::State_Sunken;

    option->features = QStyleOptionFrameV2::None;
}

void KioskCheck::mousePressEvent(QMouseEvent *event)
{
    KioskButtonBase::mousePressEvent(event);
}

void KioskCheck::mouseReleaseEvent(QMouseEvent *event)
{
    KioskButtonBase::mouseReleaseEvent(event);
    repaint();
}

void KioskCheck::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QStyleOptionFrameV2 panel;
    initStyleOption(&panel);
    KioskButtonBase::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &panel, &painter, this);

    QRect brect = style()->subElementRect(QStyle::SE_CheckBoxIndicator, &panel, this);

    if (KioskButtonBase::isChecked())
        painter.drawPixmap(QStyle::PM_DefaultFrameWidth, QStyle::PM_DefaultFrameWidth, m_checkmark);
}

KioskCheckbox::KioskCheckbox(QWidget *parent)
    : KioskButtonBase(parent),
    m_label(0)
{
    KioskButtonBase::setOpacity(0.0);

    m_layout = new QHBoxLayout(this);
    m_checkBox = new KioskCheck();

    m_layout->addWidget(m_checkBox);

    connect(m_checkBox, SIGNAL(toggled()), this, SLOT(slot_toggled()));
}

void KioskCheckbox::setText(QString text)
{
    if (!m_label) {
        m_label = new QLabel();
        m_layout->addWidget(m_label);
    }

    m_label->setText(text);
}

QString KioskCheckbox::text(void)
{
    return m_label->text();
}

void KioskCheckbox::slot_toggled(void)
{
    bool checked = m_checkBox->isChecked();
    KioskButtonBase::setChecked(checked, false);
    emit toggled();
}

void KioskCheckbox::mouseReleaseEvent(QMouseEvent *event)
{
    m_checkBox->setChecked(!m_checkBox->isChecked(), false);
    KioskButtonBase::mouseReleaseEvent(event);
}
