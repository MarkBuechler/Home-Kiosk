/* */

#include "KioskJukeboxWidget.h"

KioskJukeboxWidget::KioskJukeboxWidget(void)
    : KioskScrollArea()
{
    QWidget *main = new QWidget(this);
    KioskScrollArea::setWidget(main);
    KioskScrollArea::setWidgetResizable(true);
    QAbstractScrollArea::setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QAbstractScrollArea::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_layout = new QGridLayout(main);
    m_layout->setMargin(10);
    m_layout->setSpacing(5);
}

void KioskJukeboxWidget::setItem(int row, int col, KioskJukeboxItem *item)
{
    m_layout->addWidget(item, row, col);
}
