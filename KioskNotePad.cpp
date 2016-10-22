/* */

#include "KioskNotePad.h"

KioskStickyTrash::KioskStickyTrash(QGraphicsItem *parent)
    : QGraphicsSvgItem(parent),
    m_trash(new QSvgRenderer(QLatin1String(":/res/trash.svg")))
{
    QGraphicsSvgItem::setSharedRenderer(m_trash);
}

KioskStickyTrash::~KioskStickyTrash(void)
{
    delete m_trash;
}

KioskStickyEdit::KioskStickyEdit(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    QGraphicsTextItem::setFlag(QGraphicsItem::ItemIsSelectable, false);
    QGraphicsTextItem::moveBy(12.0, 44.0);
    QGraphicsTextItem::setRotation(-3.0);
    QGraphicsTextItem::setTextWidth(240);

    QFont font;

    font.setPointSize(16);
    font.setBold(true);
    font.setFamily("Comic Sans MS");

    QGraphicsTextItem::setFont(font);
}

void KioskStickyEdit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    QStyleOptionGraphicsItem myOption = *option;

    myOption.state &= ~QStyle::State_Selected;
    QGraphicsTextItem::paint(painter, &myOption, widget);
}

KioskStickyTack::KioskStickyTack(QGraphicsItem *parent)
    : QGraphicsSvgItem(parent),
    m_tack(new QSvgRenderer(QLatin1String(":/res/tack.svg"))),
    m_pressed(false),
    m_initialized(false)
{
    QGraphicsSvgItem::moveBy(74, 0);
    QGraphicsSvgItem::setSharedRenderer(m_tack);
    QGraphicsSvgItem::setFlag(QGraphicsItem::ItemIsMovable, true);
    QGraphicsSvgItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
    //QGraphicsSvgItem::setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
}

KioskStickyTack::~KioskStickyTack(void)
{
    delete m_tack;
}

void KioskStickyTack::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    QStyleOptionGraphicsItem myOption = *option;

    myOption.state &= ~QStyle::State_Selected;
    QGraphicsSvgItem::paint(painter, &myOption, widget);
}

void KioskStickyTack::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsSvgItem::mousePressEvent(event);

    m_pressed = true;
    m_pressPoint = event->scenePos();
}

void KioskStickyTack::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsSvgItem::mouseReleaseEvent(event);

    if (!m_initialized) {
        QPointF diff = event->scenePos() - m_pressPoint;
        if (m_pressed && (qAbs(diff.manhattanLength()) > 12))
            emit initialize(event->scenePos());
        m_initialized = true;
    } else
        emit sendToTrash(event->scenePos());

    m_pressed = false;
}

void KioskStickyTack::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsSvgItem::mouseMoveEvent(event);
}

KioskStickyNote::KioskStickyNote(QGraphicsItem *parent)
    : QGraphicsSvgItem(parent),
    m_sticky(new QSvgRenderer(QLatin1String(":/res/sticky.svg"))),
    m_textEdit(new KioskStickyEdit(this)),
    m_stickyTack(new KioskStickyTack(this)),
    m_animation(new QParallelAnimationGroup()),
    m_rotateAnimation(new QPropertyAnimation(this, "rotation")),
    m_scaleAnimation(new QPropertyAnimation(this, "scale")),
    m_trashAnimation(new QParallelAnimationGroup()),
    m_trashRotateAnimation(new QPropertyAnimation(this, "rotation")),
    m_trashMoveAnimation(new QPropertyAnimation(this, "pos")),
    m_hideAnimation(new QPropertyAnimation(this, "scale")),
    m_edited(false),
    m_editing(false),
    m_moved(false),
    m_pressed(false),
    m_animatedMove(false),
    m_animatedStop(false),
    m_initialized(false),
    m_tackX(45),
    m_tackY(50)
{
    QGraphicsSvgItem::setSharedRenderer(m_sticky);
    QGraphicsSvgItem::setFlag(QGraphicsItem::ItemIsMovable, true);
    QGraphicsSvgItem::setFlag(QGraphicsItem::ItemIsFocusable, true);
    QGraphicsSvgItem::setFlag(QGraphicsItem::ItemHasNoContents, true);
    QGraphicsSvgItem::setTransformOriginPoint(QPointF(120, 30));
    QGraphicsSvgItem::setObjectName("StickyNote");

    m_stickyTack->setScale(2.5);

    srand((unsigned)time(0));
    int rOffset = (rand()%5)+1;

    if (rOffset %2)
        rOffset = 0 - rOffset;

    qreal sOffset = rOffset / 50.0;

    //m_rotateCurve.setOvershoot(30.0);
    m_rotateCurve.setPeriod(0.3);
    m_rotateCurve.setAmplitude(5.0);

    m_rotateAnimation->setKeyValueAt(0.0, -20.0 + rOffset);
    m_rotateAnimation->setKeyValueAt(1.0, 0.0);

    m_scaleAnimation->setDuration(250);
    m_scaleAnimation->setKeyValueAt(0.0, 0.8 + sOffset);
    m_scaleAnimation->setKeyValueAt(1.0, 1.0 + sOffset);

    m_animation->addAnimation(m_rotateAnimation);
    m_animation->addAnimation(m_scaleAnimation);

    m_trashRotateAnimation->setKeyValueAt(0.0, -20.0);
    m_trashRotateAnimation->setKeyValueAt(0.5,  20.0);
    m_trashRotateAnimation->setKeyValueAt(1.0, -20.0);
    m_trashRotateAnimation->setDuration(1250);
    m_trashRotateAnimation->setLoopCount(10);

    m_trashAnimation->addAnimation(m_trashRotateAnimation);
    m_trashAnimation->addAnimation(m_trashMoveAnimation);

    m_hideAnimation->setKeyValueAt(0.0, 1.0);
    m_hideAnimation->setKeyValueAt(1.0, 0.1);
    m_hideAnimation->setDuration(250);

    connect(m_stickyTack, SIGNAL(initialize(QPointF)), this, SLOT(initialize(QPointF)));
    connect(m_stickyTack, SIGNAL(sendToTrash(QPointF)), this, SLOT(sendToTrash(QPointF)));
}

KioskStickyNote::~KioskStickyNote(void)
{
    delete m_sticky;
}

void KioskStickyNote::sendToTrash(QPointF start)
{
    m_stickyTack->hide();

    int height = qApp->desktop()->screenGeometry().height() - 100;
    QPointF end(start.x(), height);

    int moveTime = 4000 * (qAbs(height - start.y()) / height);

    m_trashMoveAnimation->setKeyValueAt(0.0, start);
    m_trashMoveAnimation->setKeyValueAt(1.0, end);
    m_trashMoveAnimation->setDuration(moveTime);

    QEasingCurve trashCurve(QEasingCurve::OutBounce);

    m_trashMoveAnimation->setEasingCurve(trashCurve);

    connect(m_trashMoveAnimation, SIGNAL(finished()), SLOT(deleteSticky()));

    m_trashAnimation->start();
}

void KioskStickyNote::deleteSticky(void)
{
    m_trashAnimation->stop();
    delete this;
}

void KioskStickyNote::hideSticky(void)
{
    m_hideAnimation->setKeyValueAt(0.0, QGraphicsSvgItem::scale());
    m_hideAnimation->setDirection(QAbstractAnimation::Forward);
    connect(m_hideAnimation, SIGNAL(finished()), this, SLOT(hideIt()));
    m_hideAnimation->start();
}

void KioskStickyNote::hideIt(void)
{
    QGraphicsSvgItem::hide();
}

void KioskStickyNote::showSticky(void)
{
    show();
    m_hideAnimation->setDirection(QAbstractAnimation::Backward);
    disconnect(m_hideAnimation, SIGNAL(finished()), this, SLOT(hideIt()));
    m_hideAnimation->start();
}

void KioskStickyNote::setEditing(bool editing)
{
    if (editing) {
        QGraphicsSvgItem::setZValue(1);

        m_textEdit->setTextInteractionFlags(Qt::TextEditorInteraction);
        m_textEdit->textCursor().movePosition(QTextCursor::End);

        if (!m_edited)
            m_textEdit->document()->clear();

        m_textEdit->setFocus();

        m_edited = true;

        m_rotateCurve.setType(QEasingCurve::OutElastic);
        m_scaleCurve.setType(QEasingCurve::OutSine);
        m_rotateAnimation->setEasingCurve(m_rotateCurve);
        m_rotateAnimation->setDuration(1250);
        m_scaleAnimation->setEasingCurve(m_scaleCurve);
        m_animation->setDirection(QAbstractAnimation::Forward);
    } else {
        QGraphicsSvgItem::setZValue(0);

        m_textEdit->textCursor().clearSelection();
        m_textEdit->setTextInteractionFlags(Qt::NoTextInteraction);

        m_rotateCurve.setType(QEasingCurve::InQuad);
        m_scaleCurve.setType(QEasingCurve::InSine);
        m_rotateAnimation->setEasingCurve(m_rotateCurve);
        m_rotateAnimation->setDuration(250);
        m_scaleAnimation->setEasingCurve(m_scaleCurve);
        m_animation->setDirection(QAbstractAnimation::Backward);
    }

    m_animation->start();
}

void KioskStickyNote::initialize(QPointF pos)
{
    if (m_initialized) return;

    m_stickyTack->setFlag(QGraphicsItem::ItemIsMovable, false);
    m_stickyTack->setPos(74, 0);
    m_stickyTack->setScale(1.0);
    QGraphicsSvgItem::setFlag(QGraphicsItem::ItemHasNoContents, false);
    QGraphicsSvgItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_textEdit->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_textEdit->setPlainText("New note..");
    m_initialized = true;

    pos.rx() -= 74;

    QGraphicsSvgItem::setPos(pos);

    emit initialized();
}

QRectF KioskStickyNote::boundingRect(void) const
{
    if (m_initialized)
        return QGraphicsSvgItem::boundingRect();

    return m_stickyTack->boundingRect();
}

void KioskStickyNote::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    QStyleOptionGraphicsItem myOption = *option;

    myOption.state &= ~QStyle::State_Selected;
    QGraphicsSvgItem::paint(painter, &myOption, widget);
}

QVariant KioskStickyNote::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        setEditing(value.toBool());
        if (!value.toBool()) emit requestHideKeyboard();
    }

    return QGraphicsSvgItem::itemChange(change, value);
}

void KioskStickyNote::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsSvgItem::mouseMoveEvent(event);

    QPointF diff = event->pos() - m_pressPoint;

#if 0
    m_moved = true;

    if (m_pressed) {
        if (!m_animatedMove && (event->pos().manhattanLength() > 1)) {
qDebug() << "animate up";
            m_rotateCurve.setType(QEasingCurve::InQuad);
            m_rotateAnimation->setDuration(1250);
            m_scaleAnimation->setEasingCurve(m_scaleCurve);
            m_animation->setDirection(QAbstractAnimation::Backward);
            m_animatedMove = true;
        } else if (!m_animatedStop) {
qDebug() << "animate STOP";
            m_rotateCurve.setType(QEasingCurve::OutElastic);
            m_rotateAnimation->setDuration(1250);
            m_rotateAnimation->setDuration(1250);
            m_animation->setDirection(QAbstractAnimation::Forward);
            m_animatedStop = true;
        }

        m_animation->start();
        m_pressed = false;
    }
#endif
}

void KioskStickyNote::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsSvgItem::mousePressEvent(event);

    m_pressed = true;
    m_animatedMove = false;
    m_animatedStop = false;
    m_pressPoint = event->pos();
}

void KioskStickyNote::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsSvgItem::mouseReleaseEvent(event);

    //if (!m_moved)
        //emit requestShowKeyboard();

    m_moved = false;
    m_pressed = false;
}

KioskGraphicsButton::KioskGraphicsButton(QGraphicsItem *parent)
    : QGraphicsSvgItem(parent), QGraphicsLayoutItem(),
    m_scaleAnimation(new QPropertyAnimation(this, "scale")),
    m_hideAnimation(new QPropertyAnimation(this, "scale")),
    m_svgRenderer(0),
    m_pressed(false),
    m_hovering(false)
{
    QGraphicsSvgItem::setAcceptHoverEvents(true);
    QGraphicsSvgItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
    QGraphicsSvgItem::setTransformOriginPoint(QPointF(32, 32));

    m_scaleAnimation->setDuration(100);
    m_scaleAnimation->setKeyValueAt(0.0, 1.0);
    m_scaleAnimation->setKeyValueAt(1.0, 0.7);

    m_hideAnimation->setKeyValueAt(0.0, 1.0);
    m_hideAnimation->setKeyValueAt(1.0, 0.1);
    m_hideAnimation->setDuration(250);
}

QSizeF KioskGraphicsButton::sizeHint(Qt::SizeHint hint, const QSizeF &rect) const
{
    return QSizeF(96, 96);
}

void KioskGraphicsButton::setGeometry(const QRectF &rect)
{
    QGraphicsSvgItem::setPos(rect.x(), rect.y());
}

void KioskGraphicsButton::setSvg(QLatin1String file)
{
    m_svgRenderer = new QSvgRenderer(file);
    QGraphicsSvgItem::setSharedRenderer(m_svgRenderer);
}

void KioskGraphicsButton::hideButton(void)
{
    m_hideAnimation->setKeyValueAt(0.0, QGraphicsSvgItem::scale());
    m_hideAnimation->setDirection(QAbstractAnimation::Forward);
    connect(m_hideAnimation, SIGNAL(finished()), this, SLOT(hideIt()));
    m_hideAnimation->start();
}

void KioskGraphicsButton::hideIt(void)
{
    QGraphicsSvgItem::hide();
}

void KioskGraphicsButton::showButton(void)
{
    show();
    m_hideAnimation->setDirection(QAbstractAnimation::Backward);
    disconnect(m_hideAnimation, SIGNAL(finished()), this, SLOT(hideIt()));
    m_hideAnimation->start();
}

void KioskGraphicsButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsSvgItem::hoverEnterEvent(event);
    m_hovering = true;
}

void KioskGraphicsButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsSvgItem::hoverLeaveEvent(event);
    m_hovering = false;
}

void KioskGraphicsButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsSvgItem::mousePressEvent(event);

    m_scaleAnimation->setDirection(QAbstractAnimation::Forward);
    m_scaleAnimation->start();

    emit pressed();
    m_pressed = true;
}

void KioskGraphicsButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsSvgItem::mouseReleaseEvent(event);

    m_scaleAnimation->setDirection(QAbstractAnimation::Backward);
    m_scaleAnimation->start();

    if (m_pressed && m_hovering)
        emit clicked();

    m_pressed = false;
}

void KioskGraphicsButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    QStyleOptionGraphicsItem myOption = *option;

    myOption.state &= ~QStyle::State_Selected;
    QGraphicsSvgItem::paint(painter, &myOption, widget);
}

