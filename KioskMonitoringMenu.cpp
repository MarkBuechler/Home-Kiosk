/* */

#include "KioskMonitoringMenu.h"

MonitoringMenu::MonitoringMenu(void)
    : KioskMenuWidget()
{
    KioskMenuWidget::setObjectName("Monitoring Menu");

    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/mon_electric.png", "Electricity", this, SLOT(slot_electric())), 0, 0);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/mon_network.png", "Network", this, SLOT(slot_network())), 0, 1);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/mon_thermostat.png", "Thermostat", this, SLOT(slot_thermostat())), 1, 1);
    KioskMenuWidget::add_button(new KioskMenuButton(this, ":/res/mon_status.png", "General", this, SLOT(slot_status())), 1, 0);
}

void MonitoringMenu::slot_electric(void)
{
    KioskMenuWidget::changeToBrowser(QUrl("http://colossus/cacti/graph_view.php?action=tree&tree_id=2&leaf_id=18&select_first=true"));
}

void MonitoringMenu::slot_network(void)
{
    KioskMenuWidget::changeToBrowser(QUrl("http://colossus:3000"));
}

void MonitoringMenu::slot_status(void)
{
    KioskMenuWidget::changeTo(new KioskStatusWidget());
}

void MonitoringMenu::slot_thermostat(void)
{
    KioskMenuWidget::changeToBrowser(QUrl("https://www.ecobee.com/home/secure/heatingAndCooling.jsf"));
}
