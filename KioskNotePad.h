/* */

#ifndef _KIOSK_NOTEPAD_H_
#define _KIOSK_NOTEPAD_H_

#include <QtGui>
#include <QtSvg>

#include "KioskGUI.h"

class KioskStickyTrash : public QGraphicsSvgItem
{
    Q_OBJECT
public:
    KioskStickyTrash(QGraphicsItem *parent=0);
    ~KioskStickyTrash(void);

private:
    QSvgRenderer *m_trash;
};

class KioskStickyEdit : public QGraphicsTextItem
{
    Q_OBJECT
public:
    KioskStickyEdit(QGraphicsItem *parent=0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
};

class KioskStickyTack : public QGraphicsSvgItem
{
    Q_OBJECT
public:
    KioskStickyTack(QGraphicsItem *parent=0);
    ~KioskStickyTack(void);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

protected slots:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QSvgRenderer *m_tack;
    QPointF m_pressPoint;

    bool m_pressed;
    bool m_initialized;

signals:
    void initialize(QPointF);
    void sendToTrash(QPointF);
};

class KioskStickyNote : public QGraphicsSvgItem
{
    Q_OBJECT
public:
    KioskStickyNote(QGraphicsItem *parent=0);
    ~KioskStickyNote(void);

    bool isInitialized(void) {return m_initialized;}

    QRectF boundingRect(void) const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void setEditing(bool editing);
    bool isEditing(void) {return m_editing;}

    void hideSticky(void);
    void showSticky(void);

public slots:
    void initialize(QPointF);
    void sendToTrash(QPointF);
    void deleteSticky(void);

protected slots:
    virtual QVariant itemChange(GraphicsItemChange change,
                                const QVariant &value);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private slots:
    void hideIt(void);

private:
    QSvgRenderer *m_sticky;

    KioskStickyEdit *m_textEdit;
    KioskStickyTack *m_stickyTack;

    QParallelAnimationGroup *m_animation;
    QPropertyAnimation *m_rotateAnimation;
    QPropertyAnimation *m_scaleAnimation;
    QParallelAnimationGroup *m_trashAnimation;
    QPropertyAnimation *m_trashRotateAnimation;
    QPropertyAnimation *m_trashMoveAnimation;
    QPropertyAnimation *m_hideAnimation;
    QEasingCurve m_rotateCurve;
    QEasingCurve m_scaleCurve;
    QPointF m_pressPoint;

    bool m_edited;
    bool m_editing;
    bool m_moved;
    bool m_pressed;
    bool m_animatedMove;
    bool m_animatedStop;
    bool m_initialized;
    qreal m_tackX;
    qreal m_tackY;

signals:
    void requestShowKeyboard(void);
    void requestHideKeyboard(void);
    void initialized(void);
};

class KioskGraphicsButton : public QGraphicsSvgItem, public QGraphicsLayoutItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsLayoutItem)
public:
    KioskGraphicsButton(QGraphicsItem *parent=0);

    void setSvg(QLatin1String file);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    virtual QSizeF sizeHint(Qt::SizeHint, const QSizeF&) const;
    virtual void setGeometry(const QRectF&);

    void hideButton(void);
    void showButton(void);

protected slots:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private slots:
    void hideIt(void);

private:
    QPropertyAnimation *m_scaleAnimation;
    QPropertyAnimation *m_hideAnimation;
    QSvgRenderer *m_svgRenderer;

    bool m_pressed;
    bool m_hovering;

signals:
    void pressed(void);
    void clicked(void);
};

#endif
