/* */

#ifndef _KIOSK_BUTTON_H_
#define _KIOSK_BUTTON_H_

#include <QtGui>

#include "KioskButtonBase.h"

enum KioskButtonStyle {
    IconOnly,
    TextOnly,
    TextUnderIcon,
    TextBesideIcon
};

class KioskButton : public KioskButtonBase
{
    Q_OBJECT
public:
    KioskButton(QWidget *parent=0);

    void setMargin(int);
    int margin(void);

    void setText(QString);
    QString text(void) {return button_text;}

    void setIcon(QIcon);
    QIcon icon(void) {return button_icon;}

    void setWidget(QWidget*);
    QWidget *widget(void) {return m_widget;}

    void setIconSize(QSize);
    QSize iconSize(void) {return icon_size;}

    QFont font(void) {return text_font;};
    void setFont(QFont);

    virtual void setEnabled(bool);

    void setButtonStyle(KioskButtonStyle);

protected:
    virtual void paintEvent(QPaintEvent*);

private:
    QPixmap _getPixmap(QIcon, QSize);

    QBoxLayout *layout;

    QLabel *text_label;
    QLabel *icon_label;

    QIcon button_icon;
    QString button_text;

    QWidget *m_widget;

    QFont text_font;
    QColor enabled_color;
    QColor disabled_color;

    QSize icon_size;

    KioskButtonStyle button_style;
};

#endif
