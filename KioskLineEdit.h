/* */

#ifndef _KIOSK_LINEEDIT_H_
#define _KIOSK_LINEEDIT_H_

#include <QtGui>

class KioskLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    KioskLineEdit(QWidget *parent=0);

    void setProgress(float);
    void setIcon(QIcon);

protected:
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void focusInEvent(QFocusEvent*);
    virtual void focusOutEvent(QFocusEvent*);
    virtual bool event(QEvent*);
    virtual void resizeEvent(QResizeEvent*);
    virtual void initStyleOption(QStyleOptionFrameV2*) const;
    virtual void paintEvent(QPaintEvent*);

private:
    void setMargins(void);

    QRect m_buttonRect;
    QIcon m_icon;

    bool  m_justCleared;
    float m_progressWidth;

signals:
    void activated(void);
    void deactivated(void);
};

#endif
