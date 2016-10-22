/* */

#include "TestMeters.h"

TestMeters::TestMeters(QWidget *parent)
    : KioskWidget()
{
    KioskWidget::setObjectName("Test");

    QVBoxLayout *top = new QVBoxLayout(this);

    QHBoxLayout *layout = new QHBoxLayout();

    dial = new KioskWattMeter();

    layout->addWidget(dial);

    edit = new QLineEdit();

    layout->addWidget(edit);

    KioskClockWidget *clock = new KioskClockWidget();

    top->addLayout(layout);
    top->addWidget(clock);

    connect(edit, SIGNAL(returnPressed()), this, SLOT(changeMeter()));

}

void TestMeters::changeMeter(void)
{
    dial->setValue(edit->text().toDouble());
}

void TestMeters::paintEvent(QPaintEvent *event)
{
    //QPainter painter(this);

    //painter.fillRect(event->rect(), Qt::gray);
}
