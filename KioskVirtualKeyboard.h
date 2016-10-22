/* */

#ifndef _KIOSK_VIRTUALKEYBOARD_
#define _KIOSK_VIRTUALKEYBOARD_

#include <QtGui>

#include "KioskGUI.h"

#include "KioskButtonBase.h"

static const unsigned char keys[][4] = {
    {0x71, 0x51, 0x31, 0x31}, // q, 1
    {0x77, 0x57, 0x32, 0x32}, // w, 2
    {0x65, 0x45, 0x33, 0x33}, // e, 3
    {0x72, 0x52, 0x34, 0x34}, // r, 4
    {0x74, 0x54, 0x35, 0x35}, // t, 5
    {0x79, 0x59, 0x36, 0x36}, // y, 6
    {0x75, 0x55, 0x37, 0x37}, // u, 7
    {0x69, 0x49, 0x38, 0x38}, // i, 8
    {0x6f, 0x4f, 0x39, 0x39}, // o, 9
    {0x70, 0x50, 0x30, 0x30}, // p, 0
    {0xff, 0xff, 0xff, 0xff}, // backspace
    {0xff, 0xff, 0xff, 0xff}, // caps
    {0x61, 0x41, 0x2b, 0x2b}, // a, +
    {0x73, 0x53, 0x2d, 0x2d}, // s, -
    {0x64, 0x44, 0x2a, 0x2a}, // d, *
    {0x66, 0x46, 0x2f, 0x2f}, // f, /
    {0x67, 0x47, 0x25, 0x2f}, // g, %
    {0x68, 0x48, 0x3d, 0x3d}, // h, =
    {0x6a, 0x4a, 0x26, 0x26}, // j, &
    {0x6b, 0x4b, 0x40, 0x40}, // k, @
    {0x6c, 0x4c, 0x00, 0x00}, // l
    {0xff, 0xff, 0xff, 0xff}, // enter
    {0x7a, 0x5a, 0x28, 0x29}, // z, (, )
    {0x78, 0x58, 0x7b, 0x7d}, // x, {, }
    {0x63, 0x43, 0x5b, 0x5d}, // c, [, ]
    {0x76, 0x56, 0x3c, 0x3e}, // v, <, >
    {0x62, 0x42, 0x7c, 0x7c}, // b, |
    {0x6e, 0x4e, 0x24, 0x24}, // n, $
    {0x6d, 0x4d, 0x5c, 0x5c}, // m, pound sterling
    {0x2c, 0x3b, 0x7e, 0x7e}, // , ;, ~
    {0x2e, 0x3a, 0x23, 0x23}, // ., :, #
    {0x3f, 0x21, 0x5e, 0x5e}, // ?, !, ^
    {0x2d, 0x5f, 0x00, 0x00}, // -, _
    {0xff, 0xff, 0xff, 0xff}, // shift
    {0xff, 0xff, 0xff, 0xff}, // alpha
    {0xff, 0xff, 0xff, 0xff}, // numeric
    {0xff, 0xff, 0xff, 0xff}, // ".com"
    {0x20, 0x20, 0x20, 0x20}, // space
    {0xff, 0xff, 0xff, 0xff}, // left, up
    {0xff, 0xff, 0xff, 0xff}, // right, down
    {0x27, 0x22, 0x27, 0x22}, // ', "
    {0xff, 0xff, 0xff, 0xff}, // ok
    {0xff, 0xff, 0xff, 0xff}, // cancel
};

class KioskKeyboardButton : public KioskButtonBase
{
    Q_OBJECT
public:
    KioskKeyboardButton(QWidget *parent=0);

    void setText(QString);
    void setSuperscriptText(QString);

    void setIcon(QIcon);
    void setSuperscriptIcon(QIcon);

    void setIconSize(QSize);
    void setSuperscriptIconSize(QSize);

protected slots:
    virtual void resizeEvent(QResizeEvent*);
    virtual void paintEvent(QPaintEvent*);

private:
    QPixmap _getPixmap(QIcon, QSize);
    QSize _textSize(QPainter*, QString);
    QPointF _center(QSize, bool);

    QString main_text;
    QString superscript_text;

    QIcon main_icon;
    QIcon superscript_icon;

    QSize main_icon_size;
    QSize superscript_icon_size;

    QPointF main_point;
    QPointF super_point;

    bool invalidate;
};

class KioskVirtualKeyboard : public QWidget
{
    Q_OBJECT
public:
    KioskVirtualKeyboard(QWidget *parent=0);
    void reset(void);

private slots:
    void slot_caps_button(void);
    void slot_shift_button(void);
    void slot_alpha_button(void);
    void slot_numeric_button(void);
    void slot_left_button(void);
    void slot_right_button(void);
    void slot_ok_button(void);
    void slot_clear_button(void);
    void slot_backspace_button(void);
    void slot_return_button(void);
    void slot_dotcom_button(void);
    void slot_button_press(int index);
    void slot_button_release(int index);

private:
    void setupUI(void);
    void setupKeys(void);

    void sendEvent(Qt::KeyboardModifiers, int, QString, int);
    void setButtonIcon(KioskKeyboardButton*, QIcon, QSize, bool superscript=false);

    bool is_shifted;
    bool is_alpha;
    bool is_numeric;
    bool is_caps;

    QHash<int, KioskKeyboardButton*> buttons;

    QWidget *vkb_parent;

signals:
    void requestHide(void);
};

#endif
