/* */

#ifndef _KIOSK_JUKEBOX_H_
#define _KIOSK_JUKEBOX_H_

#include <QtGui>

#include "KioskGUI.h"
#include "KioskJukeboxWidget.h"
#include "KioskJukeboxItem.h"
#include "KioskJukeboxSelector.h"

#include "KioskDBLink.h"

enum JukeboxState {
    StateBusy,
    StateReady,
};

class KioskJukebox : public KioskWidget
{
    Q_OBJECT
public:
    KioskJukebox(KioskDBLink *link=0);
    ~KioskJukebox(void);

private slots:
    void slot_selection_made(QString);
    void slot_random_made(int);
    void slot_read_socket(void);

private:
    void fillSelections(void);

    QTcpSocket *m_socket;

    KioskJukeboxWidget *m_selections;
    KioskJukeboxSelector *m_selector;

    JukeboxState m_state;
};

#endif

