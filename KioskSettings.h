/* */

#ifndef _KIOSK_SETTINGS_H_
#define _KIOSK_SETTINGS_H_

#include <QSettings>

class KioskSettings : public QSettings
{
    Q_OBJECT
public:
    KioskSettings(void);

    void setDimmerRange(QString, int, int);
    void dimmerRange(QString, int&, int&);
};

#endif
