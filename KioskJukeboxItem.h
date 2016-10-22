/* */

#ifndef _KIOSK_JUKEBOX_ITEM_H_
#define _KIOSK_JUKEBOX_ITEM_H_

#include <QtGui>

#include "KioskGUI.h"

class KioskJukeboxItem : public QWidget
{
    Q_OBJECT
public:
    KioskJukeboxItem(QWidget *parent=0);

    void setSelection(QString, QString);
    void setTitles(QString, QString, QString);

private slots:
    virtual void paintEvent(QPaintEvent*);

private:
    void drawLabel(QPainter *painter);
    void drawTitles(QPainter *painter);
    QSize _textSize(QPainter*, QString);
    QPointF _center(QSize, bool);

    QString m_selection1, m_selection2;
    QString m_artist, m_title1, m_title2;
};

#endif
