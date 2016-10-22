/* */

#ifndef _KIOSK_QMLITEMS_H_
#define _KIOSK_QMLITEMS_H_

#include <QtGui>

class KioskQmlItem
{
public:
    KioskQmlItem(QUrl source=QUrl(), QString name="", bool usesGL=false,
                 bool usesKeys=false, bool transparent=false)
    {m_source = source; m_name = name; m_usesGL = usesGL; m_usesKeys = usesKeys;
     m_transparent = transparent;}

    void setSource(QUrl source) {m_source = source;}
    void setName(QString name) {m_name = name;}
    void setUsesGL(bool usesGL) {m_usesGL = usesGL;}
    void setUsesKeys(bool usesKeys) {m_usesKeys = usesKeys;}
    void setTransparent(bool transparent) { m_transparent = transparent;}

    QUrl source(void) {return m_source;}
    QString name(void) {return m_name;}
    bool usesGL(void) {return m_usesGL;}
    bool usesKeys(void) {return m_usesKeys;}
    bool transparent(void) {return m_transparent;}

private:
    QUrl m_source;
    QString m_name;
    bool m_usesGL;
    bool m_usesKeys;
    bool m_transparent;
};

class KioskNewsViewer : public KioskQmlItem
{
public:
    KioskNewsViewer(void) : KioskQmlItem(QUrl("qrc:/rssnews.qml"),
                                         "News Viewer", true, true, false) {};
};

class KioskPhotoViewer : public KioskQmlItem
{
public:
    KioskPhotoViewer(void) : KioskQmlItem(QUrl("qrc:/photoviewer.qml"),
                                          "Flickr Viewer", true, true, false) {};
};

#endif
