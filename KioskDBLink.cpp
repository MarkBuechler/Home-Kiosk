/* */

#include "KioskDBLink.h"

KioskDBLink::KioskDBLink(void)
    : QObject()
{
}

void KioskDBLink::connect(void)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName(DB_HOST);
    m_db.setDatabaseName(DB_NAME);
    m_db.setUserName(DB_USER);
    m_db.setPassword(DB_PASS);
    if (!m_db.open()) {
        qDebug() << "Failed to connect to mysql.";
        qDebug() << qPrintable(m_db.lastError().text());
    }
}

void KioskDBLink::disconnect(void)
{
    m_db.close();
}

QHash<QString, JukeboxRecord> KioskDBLink::queryJukebox(void)
{
    QHash<QString, JukeboxRecord> entries;

    if (!m_db.isOpen())
        return entries;

    QSqlQuery q = m_db.exec(
"SELECT letter, number, artist, title FROM jukebox ORDER BY letter, number"
    );
qDebug() << qPrintable(m_db.lastError().text());
    while (q.next()) {
        JukeboxRecord record;

        record.letter = q.value(0).toString();
        record.number = q.value(1).toInt();
        record.artist = q.value(2).toString();
        record.title  = q.value(3).toString();

        entries.insert(QString("%1%2").arg(record.letter).arg(record.number), record);
    }

    return entries;
}
