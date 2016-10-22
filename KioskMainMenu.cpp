/* */

#include "KioskMainMenu.h"

MainMenu::MainMenu(KioskDBLink *link)
    : KioskMenuWidget(link)
{
    KioskMenuWidget::setObjectName("Main Menu");

    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/lights.png", "Lighting", this, SLOT(slot_lights())), 0, 0);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/weather.png", "Weather", this, SLOT(slot_weather())), 1, 1);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/security.png", "Security", this, SLOT(slot_security())), 0, 2);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/appliances.png", "Appliances", this, SLOT(slot_appliances())), 0, 1);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/monitoring.png", "Monitoring", this, SLOT(slot_monitoring())), 1, 0);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/webkit.png", "Web Browser", this, SLOT(slot_browser())), 2, 1);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/media.png", "Media", this, SLOT(slot_media())), 2, 0);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/utility.png", "Utility", this, SLOT(slot_utility())), 2, 2);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/gm.png", "Maps", this, SLOT(slot_maps())), 1, 2);
}

void MainMenu::slot_lights(void)
{
    KioskWidget::changeTo(new LightingMenu());
}

void MainMenu::slot_weather(void)
{
    KioskWidget::changeToBrowser(QUrl("http://www.weather.com/weather/today/Pittsford+NY+14534"));
}

void MainMenu::slot_security(void)
{
}

void MainMenu::slot_appliances(void)
{
}

void MainMenu::slot_monitoring(void)
{
    KioskWidget::changeTo(new MonitoringMenu());
}

void MainMenu::slot_browser(void)
{
    KioskWidget::changeToBrowser(QUrl("http://www.buechlers.com"));
}

void MainMenu::slot_media(void)
{
    KioskWidget::changeTo(new MediaMenu(dbLink()));
}

void MainMenu::slot_utility(void)
{
    KioskWidget::changeTo(new UtilityMenu());
}

void MainMenu::slot_maps(void)
{
#if 0
    KioskWidget::changeToBrowser(QUrl("http://maps.google.com/maps?f=q&source=s_q&hl=en&geocode=&"
      "q=250+Stone+Rd,+Pittsford,+NY+14534&aq=0&sll=37.09024,-112.5&"
      "sspn=40.324283,92.109375&ie=UTF8&hq=&"
      "hnear=250+Stone+Rd,+Pittsford,+Monroe,+New+York+14534&t=h&z=16"));
#endif

    QProcess *process = new QProcess();
    process->start("googleearth");
}

