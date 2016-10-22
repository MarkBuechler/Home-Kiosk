/* */

#ifndef _KIOSK_CHECKBOX_H_
#define _KIOSK_CHECKBOX_H_

#include <QtGui>

#include "KioskGUI.h"

class KioskCheck : public KioskButtonBase
{
    Q_OBJECT
public:
    KioskCheck(QWidget *parent=0);

protected slots:
    virtual void initStyleOption(QStyleOptionFrameV2*) const;
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void paintEvent(QPaintEvent*);

private:
    QPixmap m_checkmark;
};

class KioskCheckbox : public KioskButtonBase
{
    Q_OBJECT
public:
    KioskCheckbox(QWidget *parent=0);

    void setText(QString text);
    QString text(void);

protected slots:
    virtual void mouseReleaseEvent(QMouseEvent*);

private slots:
    void slot_toggled(void);

private:
    QHBoxLayout *m_layout;
    QLabel *m_label;

    KioskCheck *m_checkBox;
};

#endif
