/* */

#include "KioskStatusWidget.h"

KioskStatusWidget::KioskStatusWidget(QWidget *parent)
    : KioskWidget()
{
    KioskWidget::setUsesKeyboard(false);
    KioskWidget::setStatusScreen(true);

    QHBoxLayout *top = new QHBoxLayout(this);
    top->setMargin(0);

    KioskElectricMeters *meters = new KioskElectricMeters();

    QVBoxLayout *left = new QVBoxLayout();
    left->setMargin(0);

    left->addWidget(meters, 3);
    left->addWidget(new QLabel(), 1);

    top->addLayout(left, 4);

    QVBoxLayout *right = new QVBoxLayout();
    right->setMargin(0);

    KioskClockWidget *clock = new KioskClockWidget();

    right->addWidget(clock);

    web = new KioskWebView();

    QPalette palette = web->palette();
    palette.setBrush(QPalette::Background, Qt::transparent);
    palette.setBrush(QPalette::Base, Qt::transparent);
    web->page()->setPalette(palette);
    web->setAttribute(Qt::WA_OpaquePaintEvent, false);
    web->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    web->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    web->load(QUrl("qrc:/res/placeholder.html"));

    right->addWidget(web, 5);

    top->addLayout(right, 1);
}
