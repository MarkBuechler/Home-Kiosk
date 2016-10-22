/* */

#ifndef _KIOSK_GUI_H_
#define _KIOSK_GUI_H_

#include <cmath>
#include <cstdlib>
#include <ctime>

#include <QtGui>
#include <QtOpenGL>
#include <QtDeclarative>
#include <QtScroller>
#include <QtScrollerProperties>

#include "KioskButton.h"
#include "KioskLightSwitch.h"
#include "KioskQmlItems.h"

#include "KioskDBLink.h"

class KioskWebBrowser;

class KioskClock : public QWidget
{
    Q_OBJECT
public:
    KioskClock(QWidget *parent=0);
};

class KioskGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    KioskGraphicsView(QWidget *parent=0);

    void grabGesture(QtScroller::ScrollerGestureType);
    void ungrabGesture(void);
    void setOvershoot(bool);

    QtScroller *scroller(void) {return m_scroller;}

private:
    QtScroller *m_scroller;
};

class KioskDeclarativeView : public QDeclarativeView
{
    Q_OBJECT
public:
    KioskDeclarativeView(QWidget *parent=0);

    QObject *setQmlItem(KioskQmlItem *item);

    KioskQmlItem *qmlItem(void) {return m_item;}
    void reset(void);

private:
    KioskQmlItem *m_item;
};

class KioskScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    KioskScrollArea(QWidget *parent=0);

    void grabGesture(QtScroller::ScrollerGestureType type);
    void ungrabGesture(void);
    void setOvershoot(bool overshoot);

    QtScroller *scroller(void) {return m_scroller;}

private:
    QtScroller *m_scroller;
};

enum LightType {
    TYPE_LIGHT,
    TYPE_SCENE,
};

class KioskLightStatus : public QWidget
{
    Q_OBJECT
public:
    KioskLightStatus(QWidget *parent=0);

    void setLevel(int _level);
    int level(void) {return m_level;}

protected slots:
    virtual void paintEvent(QPaintEvent *event);

private:
    int m_level;
};

class KioskSliderButton: public QWidget
{
    Q_OBJECT
public:
    KioskSliderButton(QWidget *parent=0, LightType _type=TYPE_LIGHT, QString _name="",
        QString text="", bool dimmable=false, int initial_value=0, QObject *caller=0,
        const char *button_slot=0, const char *slider_slot=0);
    void setEnabled(bool _enable);
    void setType(LightType _type) {m_type = _type;}
    void setLevel(int _level);

    bool isEnabled(void) {return m_enabled;}
    bool isDimmable(void) {return m_dimmable;}
    LightType type(void) {return m_type;}
    int level(void);
    QString name(void) {return m_name;}

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *event);

private slots:
    void slot_button_clicked(void);
    void slot_slider_changed(void);
    void slot_slider_moved(int value);

private:
    KioskButton *m_button;
    KioskLightStatus *m_light;

    QWidget *m_slider;
    QVBoxLayout *m_top;
    QIcon m_onImage;
    QIcon m_offImage;
    QString m_name;

    LightType m_type;

    bool m_buttonState;
    bool m_dimmable;
    bool m_enabled;

signals:
    void buttonClicked(const QString name, const LightType type);
    void sliderChanged(const QString name, const LightType type);
};

class KioskMenuButton : public KioskButton
{
    Q_OBJECT
public:
    KioskMenuButton(QWidget *parent=0, QString image="", QString text="",
        QObject *caller=0, const char *slot=0);
};

class KioskToolButton : public KioskButton
{
    Q_OBJECT
public:
    KioskToolButton(QWidget *parent=0, QString image="", QString text="",
        QObject *caller=0, const char *slot=0);
};

class KioskToolbar : public QWidget
{
    Q_OBJECT
public:
    KioskToolbar(QWidget *parent=0);
    void insertWidget(int index, QWidget *widget, int stretch=0, Qt::Alignment alignment=0);
    void insertSpacerItem(int index, QSpacerItem *spacerItem);
    KioskToolButton *previousButton(void) {return previous_button;}

public slots:
    void slot_previous(void);

private:
    QHBoxLayout *toolbar_layout;

    KioskToolButton *previous_button;

signals:
    void previousWindow(void);
};

class KioskWidget : public QWidget
{
    Q_OBJECT
public:
    KioskWidget(KioskDBLink *link=0);
    void setUsesKeyboard(bool uses) {uses_keyboard = uses;}
    bool usesKeyboard(void) {return uses_keyboard;}

    void setUsesNotepad(bool uses) {uses_notepad = uses;}
    bool usesNotepad(void) {return uses_notepad;}

    void setStatusScreen(bool is) {is_status = is;}
    bool isStatusScreen(void) {return is_status;}

    void setDBLink(KioskDBLink *link) {m_db = link;}
    KioskDBLink *dbLink(void) {return m_db;}

protected:
    void changeTo(QWidget *widget);
    void changeToBrowser(QUrl url);
    void changeToQml(KioskQmlItem *item);

private:
    bool uses_keyboard;
    bool uses_notepad;
    bool is_status;

    KioskDBLink *m_db;

signals:
    void changeKiosk(QWidget *widget);
    void changeBrowser(QUrl url);
    void changeQml(KioskQmlItem *item);
    void requestShowKeyboard(void);
    void requestHideKeyboard(void);
};

class KioskMenuWidget : public KioskWidget
{
    Q_OBJECT
public:
    KioskMenuWidget(KioskDBLink *link=0);
    void add_button(QWidget *button, int row, int col);

protected slots:
    virtual void paintEvent(QPaintEvent *event);

private:
    QGridLayout *menu_layout;
};

class KioskStyle : public QGtkStyle
{
public:
    KioskStyle(void);
    int pixelMetric(PixelMetric metric, const QStyleOption *option = 0, const QWidget *widget = 0) const;
};

class KioskHeaderLabel : public QLabel
{
    Q_OBJECT
public:
    KioskHeaderLabel(QWidget *parent=0);

protected slots:
    void paintEvent(QPaintEvent *event);
};

#endif

