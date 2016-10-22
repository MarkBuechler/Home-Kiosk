/* */

#include "KioskBackground.h"

KioskBackground::KioskBackground(void)
    : QWidget()
{
    QPalette palette = QWidget::palette();
    palette.setColor(QPalette::Background, Qt::gray);
    QWidget::setPalette(palette);

    QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    container = new QX11EmbedContainer();

    layout->setMargin(0);
    layout->addWidget(container);
}

KioskBackground::~KioskBackground(void)
{
    if (process && (process->state() == QProcess::Running))
        process->kill();
}

void KioskBackground::setApp(QString app)
{
    uint xid = container->winId();

    process = new QProcess();

    QStringList args;
    args << "-window-id" << QString::number(xid);

    if (!app.isEmpty())
        process->start(app, args);
}
