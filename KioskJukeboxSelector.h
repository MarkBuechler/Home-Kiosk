/* */

#ifndef _KIOSK_JUKEBOX_SELECTOR_H_
#define _KIOSK_JUKEBOX_SELECTOR_H_

#include <QtGui>

#include "KioskGUI.h"

#include "KioskButtonBase.h"

enum JukeboxButtonType {
    ButtonTypeLetter,
    ButtonTypeNumber,
    ButtonTypeReset,
    ButtonTypeRandom,
};

class KioskJukeboxSelectorButton : public KioskButtonBase
{
    Q_OBJECT
public:
    KioskJukeboxSelectorButton(QWidget *parent=0);

    void setText(QString);
    void setType(JukeboxButtonType);

    QString text(void) {return main_text;}

protected slots:
    virtual void paintEvent(QPaintEvent*);

private:
    QSize _textSize(QPainter*, QString);
    QPointF _center(QSize, bool);

    QString main_text;
    QPointF main_point;

    QString m_background;

    JukeboxButtonType m_type;
};

class KioskJukeboxWaitLight : public QWidget
{
    Q_OBJECT
public:
    KioskJukeboxWaitLight(void);

    void setLit(bool);

protected slots:
    virtual void paintEvent(QPaintEvent*);

private:
    bool m_lit;
};

class KioskJukeboxSelector : public QWidget
{
    Q_OBJECT
public:
    KioskJukeboxSelector(QWidget *parent=0);

    void reset(void);
    void setBusy(bool);

protected slots:
    void paintEvent(QPaintEvent*);

private slots:
    void _reset(void);
    void slot_letter_press(int index);
    void slot_number_press(int index);
    void slot_random(void);

private:
    void signalSelection(void);

    bool m_letterPressed;
    bool m_numberPressed;
    bool m_busy;

    KioskJukeboxWaitLight *m_waitLight;
    KioskJukeboxSelectorButton *m_resetButton;
    KioskJukeboxSelectorButton *m_randomButton;

    QHash<int, KioskJukeboxSelectorButton*> letters;
    QHash<int, KioskJukeboxSelectorButton*> numbers;

signals:
    void selectionMade(QString);
    void randomMade(int);
};

#endif
