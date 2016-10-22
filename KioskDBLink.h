/* */

#ifndef _KIOSK_DBLINK_H_
#define _KIOSK_DBLINK_H_

#include <QObject>
#include <QString>
#include <QHash>
#include <QtSql>

#define DB_HOST "colossus"
#define DB_NAME "kiosk"
#define DB_USER "kiosk"
#define DB_PASS "kiosk"

struct JukeboxRecord {
    QString letter;
    int number;
    QString artist;
    QString title;
};

class KioskDBLink : public QObject
{
    Q_OBJECT
public:
    KioskDBLink(void);

    void connect(void);
    void disconnect(void);

    bool isConnected(void) {return m_db.isOpen();}

    QHash<QString, JukeboxRecord> queryJukebox(void);

private:
    QSqlDatabase m_db;
};

#endif
