/* */

#include "KioskNotes.h"

KioskNotes::KioskNotes(void)
    : KioskWidget()
{
    KioskWidget::setObjectName("Slicky Notes");

    KioskWidget::setUsesKeyboard(true);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(0);

    QGLFormat format = QGLFormat::defaultFormat();
    format.setSampleBuffers(false);
    QGLWidget *glWidget = new QGLWidget(format);

    QDeclarativeView *qmlView = new QDeclarativeView();

    //glWidget->setAutoFillBackground(false);
    //qmlView->setViewport(glWidget);

    qmlView->viewport()->setAutoFillBackground(false);
    qmlView->setSource(QUrl("qrc:/corkboards.qml"));
    qmlView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    QObject *item = qmlView->rootObject();

    vbox->addWidget(qmlView, 7);
}

void KioskNotes::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    painter.setOpacity(0.35);
    painter.fillRect(event->rect(), Qt::black);

    KioskWidget::paintEvent(event);
}
