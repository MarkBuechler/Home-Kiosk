/* */

#ifndef _KIOSK_LIGHTINGMENU_H_
#define _KIOSK_LIGHTINGMENU_H_

#include <stdio.h>
#include <QtGui>
#include <QtNetwork>

#include "KioskGUI.h"
#include "KioskLightScheduleEditor.h"
#include "KioskSettings.h"

class LightingMenu : public KioskMenuWidget
{
    Q_OBJECT
public:
    LightingMenu(void);

private slots:
    void slot_read_socket(void);
    void slot_light_schedule(const QString, const LightType);
    void slot_light_slider(const QString, const LightType);

private:
    void check_light_button(bool, LightType,
      QString, QString, uint);

    KioskSettings *m_settings;

    QTcpSocket *socket;

    QHash<QString, KioskSliderButton*> lights;
    QHash<QString, bool> m_skipUpdate;

    int row;
    int column;
};

#endif
