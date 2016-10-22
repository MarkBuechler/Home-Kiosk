/* */

#ifndef _KIOSK_BUTTONBASE_H_
#define _KIOSK_BUTTONBASE_H_

#include <QtGui>

enum KioskKeyboardButtonState {
    ButtonStateReleased,
    ButtonStatePressed,
    ButtonStateChecked,
    ButtonStateUnchecked,
    ButtonStateDisabled,
};

enum KioskKeyboardButtonType {
    ButtonTypeNormal,
    ButtonTypeOkClose,
};

class ButtonGradColor
{
public:
    ButtonGradColor(QColor start=Qt::white, QColor end=Qt::black)
        {_start = start; _end = end;}
    QColor start(void) {return _start;}
    QColor end(void) {return _end;}

    void setStart(QColor start) {_start = start;}
    void setEnd(QColor end) {_end = end;}

private:
    QColor _start;
    QColor _end;
};

class KioskButtonBase : public QWidget
{
    Q_OBJECT
public:
    KioskButtonBase(QWidget *parent=0);

    void setButtonType(KioskKeyboardButtonType);

    void setCheckable(bool check) {is_checkable = check;}
    bool checkable(void) {return is_checkable;}

    void setChecked(bool, bool emitter=true);
    bool isChecked(void) {return (button_state == ButtonStateChecked);}

    bool isPressed(void) {return is_pressed;}

    void setAutoRepeat(bool repeat) {is_autorepeat = repeat;}
    bool isAutoRepeat(void) {return is_autorepeat;}

    void setAutoRepeatDelay(int delay) {autorepeat_delay = delay;}
    int autorepeatDelay(void) {return autorepeat_delay;}

    void setAutoRepeatInterval(int interval) {autorepeat_interval = interval;}
    int autorepeatInterval(void) {return autorepeat_interval;}

    void setMutual(bool mutual);

    void setOpacity(float opacity);
    float opacity(void) {return button_opacity;}

public slots:
    void toggle(void);

protected slots:
    virtual void changeEvent(QEvent*);
    virtual void enterEvent(QEvent*);
    virtual void leaveEvent(QEvent*);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void paintEvent(QPaintEvent*);

private slots:
    void slot_check_autorepeat(void);

private:
    void _setup(void);

    QHash<int, QHash<int, ButtonGradColor*> > gradients;

    KioskKeyboardButtonState button_state;
    KioskKeyboardButtonType  button_type;

    QTimer *autorepeat_timer;

    bool is_checkable;
    bool is_pressed;
    bool is_checked;
    bool is_enabled;
    bool is_mutual;
    bool is_autorepeat;
    bool is_hovering;
    bool did_hover;

    float button_opacity;

    int autorepeat_delay;
    int autorepeat_interval;

signals:
    void clicked(void);
    void pressed(void);
    void released(void);
    void toggled(void);
};

#endif
