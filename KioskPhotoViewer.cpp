/* */

#include "KioskPhotoViewer.h"

KioskPhotoViewer::KioskPhotoViewer(void)
    : KioskWidget()
{
    KioskWidget::setObjectName("Photo Viewer");

    KioskWidget::setUsesKeyboard(true);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(0);

    QGLFormat format = QGLFormat::defaultFormat();
    format.setSampleBuffers(false);
    QGLWidget *glWidget = new QGLWidget(format);

    QDeclarativeView *qmlView = new QDeclarativeView();

    //glWidget->setAutoFillBackground(false);
    qmlView->setViewport(glWidget);

    //qmlView->viewport()->setAutoFillBackground(false);
    qmlView->setSource(QUrl("qrc:/photoviewer.qml"));
    qmlView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    QObject *item = qmlView->rootObject();

    vbox->addWidget(qmlView, 7);
}

void KioskPhotoViewer::paintEvent(QPaintEvent *event)
{
#if 0
    QPainter painter(this);
    painter.setOpacity(0.75);
    painter.fillRect(event->rect(), Qt::black);
#endif
    KioskWidget::paintEvent(event);
}
