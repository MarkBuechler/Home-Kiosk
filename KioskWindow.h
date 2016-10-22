/* */

#ifndef _KIOSK_WINDOW_H_
#define _KIOSK_WINDOW_H_

#include <QtGui>

#include "KioskGUI.h"
#include "KioskWebBrowser.h"
#include "KioskVirtualKeyboard.h"
#include "KioskStatusWidget.h"
#include "KioskNotePad.h"
#include "KioskQmlItems.h"
#include "KioskDBLink.h"

class KioskEventFilter : public QObject
{
  Q_OBJECT
public:
    KioskEventFilter(QObject *parent=0) : QObject(parent)
    {
        is_pressed = false;
    }

protected:
    bool eventFilter(QObject *object, QEvent *event)
    {
        if (event->type() == QEvent::KeyPress) {
            emit resetIdle();
        } else if (event->type() == QEvent::MouseMove) {
            emit resetIdle();
            mouseP = ((QMouseEvent*)event)->globalPos();
//            if (is_pressed)
//                paintFlare();
        } else if (event->type() == QEvent::MouseButtonPress) {
//            is_pressed = true;
            QString className = object->metaObject()->className();
            if (!className.compare("KioskLineEdit"))
                emit showKeyboard();
//            paintFlare();
        } else if (event->type() == QEvent::MouseButtonRelease) {
//            is_pressed = false;
//            paintFlare();
#if 0
        } else if (event->type() == QEvent::Paint) {
            bool ret = QObject::eventFilter(object, event);
            paintFlare();
            return ret;
#endif
        }

        return QObject::eventFilter(object, event);
    }
// This needs to be moved to something which can paint
    void paintFlare(void)
    {
#if 0
        if (is_pressed) {
            QPainter painter(this);

            QRadialGradient gradient(mouseP, 50, mouseP);

            gradient.setColorAt(0.0, QColor(255, 255, 255, 255));
            gradient.setColorAt(0.1, QColor(255, 255, 255, 255));
            gradient.setColorAt(0.2, QColor(255, 176, 176, 167));
            gradient.setColorAt(0.3, QColor(255, 151, 151, 92));
            gradient.setColorAt(0.4, QColor(255, 125, 125, 51));
            gradient.setColorAt(0.5, QColor(255, 76, 76, 205));
            gradient.setColorAt(0.52, QColor(255, 76, 76, 205));
            gradient.setColorAt(0.6, QColor(255, 180, 180, 84));
            gradient.setColorAt(1.0, QColor(255, 255, 255, 0));
            gradient.setSpread(QGradient::PadSpread);

            repaintRect = QRect(mouseP.x() - 50, mouseP.y() - 50, 100, 100);

            painter.fillRect(repaintRect, gradient);
        }
#endif
    }

private:
    QPoint mouseP;
    QRect repaintRect;

    bool is_pressed;

signals:
    void resetIdle(void);
    void showKeyboard(void);
};

class KioskWindow : public QWidget
{
    Q_OBJECT
public:
    KioskWindow(QWidget *parent=0, bool cursor=false);

    KioskWebBrowser *browserWidget(void) {return browser_widget;}
    KioskVirtualKeyboard *keyboardWidget(void) {return keyboard_widget;}

public slots:
    void addWidget(QWidget*);
    void showBrowser(QUrl);
    void showQml(KioskQmlItem*);

protected slots:
    void hideKeyboard(void);
    void showKeyboard(void);

    void hideToolbar(void);
    void showToolbar(void);

    void hideBrowser(void);

    void hideQml(void);

    virtual void resizeEvent(QResizeEvent*);

private slots:
    void slot_previous(void);
    void slot_keyboard(void);
    void slot_keyboard_hide_request(void);
    void slot_keyboard_show_request(void);
    void slot_reset_idle(void);
    void slot_idle(void);
    void slot_new_sticky(void);
    void slot_toggle_notes(void);

private:
    void _drawFlare(QPaintEvent*);

    QSplitter *split;
    QStackedWidget *stack;
    QWidget *browser_tools;
    QPoint mouseP;
    QRect repaintRect;
    QGraphicsScene *m_scene;
    QGraphicsProxyWidget *m_stackProxy;
    QSettings *m_settings;

    QTimer *idleTimer;

    KioskDeclarativeView *m_qmlViewer;
    KioskWindow *kiosk_parent;
    KioskToolbar *toolbar_widget;
    KioskWebBrowser *browser_widget;
    KioskVirtualKeyboard *keyboard_widget;
    KioskToolButton *keyboard_button;
    KioskGraphicsView *m_scroller;
    KioskGraphicsButton *m_notepadButton;
    KioskGraphicsButton *m_keyboardButton;

    KioskEventFilter *eventFilter;

    KioskDBLink *m_db;

    bool is_pressed;
    bool is_idled;
    bool m_first;
    bool m_notesVisible;
};

#endif
