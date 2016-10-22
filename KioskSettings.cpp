/* */

#include "KioskSettings.h"

#define DIMMER_RANGE_GROUP "Dimmer_Ranges"
#define DIMMER_RANGE_LOW   "Minimum"
#define DIMMER_RANGE_HIGH  "Maximum"

KioskSettings::KioskSettings(void)
    : QSettings()
{
}

void KioskSettings::setDimmerRange(QString name, int min, int max)
{
    QSettings::beginGroup(DIMMER_RANGE_GROUP);
    QSettings::setValue(QString("%1/%2").arg(name).arg(DIMMER_RANGE_LOW), min);
    QSettings::setValue(QString("%1/%2").arg(name).arg(DIMMER_RANGE_HIGH), max);
    QSettings::endGroup();

    QSettings::sync();
}

void KioskSettings::dimmerRange(QString name, int &min, int &max)
{
    QSettings::beginGroup(DIMMER_RANGE_GROUP);

    min = QSettings::value(QString("%1/%2").arg(name).arg(DIMMER_RANGE_LOW), 0).toInt();
    max = QSettings::value(QString("%1/%2").arg(name).arg(DIMMER_RANGE_HIGH), 100).toInt();

    QSettings::endGroup();
}
