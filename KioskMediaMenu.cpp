/* */

#include "KioskMediaMenu.h"

MediaMenu::MediaMenu(KioskDBLink *link)
    : KioskMenuWidget(link)
{
    KioskMenuWidget::setObjectName("Media Menu");

    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/mythtv.png", "MythTV", this, SLOT(slot_mythtv())), 0, 0);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/netflix.png", "NetFlix", this, SLOT(slot_netflix())), 0, 1);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/jukebox.png", "AMI Jukebox", this, SLOT(slot_jukebox())), 1, 0);
}

void MediaMenu::slot_mythtv(void)
{
    KioskWidget::changeToBrowser(QUrl("http://colossus/mythweb/"));
}

void MediaMenu::slot_netflix(void)
{
    KioskMenuWidget::changeToBrowser(QUrl("http://www.netflix.com"));
}

void MediaMenu::slot_jukebox(void)
{
    KioskMenuWidget::changeTo(new KioskJukebox(dbLink()));
}
