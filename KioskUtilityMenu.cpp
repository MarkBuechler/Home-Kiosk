/* */

#include "KioskUtilityMenu.h"

UtilityMenu::UtilityMenu(void)
    : KioskMenuWidget()
{
    KioskMenuWidget::setObjectName("Utility Menu");

    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/calendar.png", "Calendar", this, SLOT(slot_calendar())), 0, 0);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/photoviewer.png", "Flickr", this, SLOT(slot_photoviewer())), 0, 1);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/newsviewer.png", "News", this, SLOT(slot_newsviewer())), 1, 0);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/exit.png", "Exit Kiosk", this, SLOT(slot_exit())), 1, 1);
}

void UtilityMenu::slot_calendar(void)
{
    KioskMenuWidget::changeTo(new KioskCalendar());
}

void UtilityMenu::slot_photoviewer(void)
{
    KioskMenuWidget::changeToQml(new KioskPhotoViewer());
}

void UtilityMenu::slot_newsviewer(void)
{
    KioskMenuWidget::changeToQml(new KioskNewsViewer());
}

void UtilityMenu::slot_exit(void)
{
    qApp->exit(0);
}
