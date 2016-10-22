/* */

#include "KioskJukebox.h"

KioskJukebox::KioskJukebox(KioskDBLink *link)
    : KioskWidget(link)
{
    KioskWidget::setObjectName("Jukebox");
    KioskWidget::setUsesKeyboard(false);
    KioskWidget::setUsesNotepad(false);

    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setMargin(0);

    m_selections = new KioskJukeboxWidget();

    hbox->addWidget(m_selections, 2);

    m_selector = new KioskJukeboxSelector();

    hbox->addWidget(m_selector);

    fillSelections();

    m_socket = new QTcpSocket();

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(slot_read_socket()));

    m_socket->connectToHost("ami_jukebox", 5000);

    connect(m_selector, SIGNAL(selectionMade(QString)), this, SLOT(slot_selection_made(QString)));
    connect(m_selector, SIGNAL(randomMade(int)), this, SLOT(slot_random_made(int)));
}

KioskJukebox::~KioskJukebox(void)
{
    if (dbLink() && dbLink()->isConnected())
        dbLink()->disconnect();
}

void KioskJukebox::fillSelections(void)
{
    int letter = 0;
    int number = 0;

    if (!dbLink())
        return;

    QHash<QString, JukeboxRecord> records = dbLink()->queryJukebox();

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 25; y++) {
            KioskJukeboxItem *item = new KioskJukeboxItem(m_selections);

            number++;
            if (number == 21) {
                number = 1;
                letter++;

                if (letter == 8)
                    letter++;
            }

            QString selection1 = QString("%1%2").arg(QChar(letter+0x41)).arg(number);
            QString selection2 = QString("%1%2").arg(QChar(letter+0x41)).arg(++number);

            item->setSelection(selection1, selection2);
            m_selections->setItem(y, x, item);

            JukeboxRecord title1, title2;
            QString a;

            if (records.contains(selection1)) {
                JukeboxRecord record = records[selection1];
                title1 = record;
            }

            if (records.contains(selection2)) {
                JukeboxRecord record = records[selection2];
                title2 = record;
            }

            if (!title1.artist.compare(title2.artist))
                a = title1.artist;
            else
                a = QString("%1 / %2").arg(title1.artist).arg(title2.artist);

            item->setTitles(a, title1.title, title2.title);
        }
    }
}

void KioskJukebox::slot_read_socket(void)
{
    QString line;

    while(m_socket->canReadLine()) {
        line = m_socket->readLine().trimmed();

        QStringList list = line.split(":");

        QString _state = list[0];
        QString text = list[1];

        if (!_state.compare("busy"))
            m_state = StateBusy;
        else if (!_state.compare("ready"))
            m_state = StateReady;
    }

    m_selector->setBusy(m_state == StateBusy);
}

void KioskJukebox::slot_selection_made(QString selection) {
    // send it off
    QString command = QString("select %1.").arg(selection);

qDebug() << qPrintable(QString(command));

    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(qPrintable(command), command.length());
        m_socket->flush();
    }

    m_selector->reset();
}

void KioskJukebox::slot_random_made(int number) {
    // send it off
    QString command = QString("random %1.").arg(number);

qDebug() << qPrintable(QString(command));

    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(qPrintable(command), command.length());
        m_socket->flush();
    }
    m_selector->reset();
}
