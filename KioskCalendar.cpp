/* */

#include "KioskCalendar.h"

KioskCalendar::KioskCalendar(void)
    : KioskWidget()
{
    KioskWidget::setObjectName("Calendar");
    KioskWidget::setUsesKeyboard(true);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(0);

    QCalendarWidget *calendar = new QCalendarWidget();
    calendar->setNavigationBarVisible(true);
    calendar->setGridVisible(true);

    vbox->addWidget(calendar);
}
