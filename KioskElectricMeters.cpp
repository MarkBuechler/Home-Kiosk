/* */

#include "KioskElectricMeters.h"

KioskElectricMeters::KioskElectricMeters(QWidget *parent)
    : QWidget(parent)
{
    last_row = 0;
    last_column = 0;
    height_hint = 0;

    top = new QVBoxLayout(this);
    top->setMargin(0);

    label = new KioskHeaderLabel();
    label->setText("Real-time Power Utilization");

    top->addWidget(label);

    KioskScrollArea *scroller = new KioskScrollArea();

    QWidget *metersW = new QWidget();
    scroller->setWidget(metersW);
    scroller->setStyleSheet("background: transparent");
    scroller->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroller->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    top->addWidget(scroller);

    layout = new QGridLayout(metersW);
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->setMargin(0);
    layout->setSpacing(2);

    addMeter("1.Ch1_Watts",  "Total Electric", WATT_METER);
    addMeter("1.Aux4_Watts", "Bar & Kitchen", WATT_METER);
    addMeter("1.Aux2_Watts", "Bedrooms", WATT_METER);
    addMeter("1.Aux1_Watts", "Computer Lab", WATT_METER);
    addMeter("1.Volts",      "Phase Voltage", VOLT_METER);
    addMeter("1.Aux5_Watts", "Main Furnace", WATT_METER);
    addMeter("1.Aux3_Watts", "All Pumps", WATT_METER);
    addMeter("1.Ch2_Watts",  "Hot Tub", WATT_METER);
    addMeter("1.Remaining",  "Remaining Circuits", WATT_METER);

    socket = new QTcpSocket();

    connect(socket, SIGNAL(readyRead()), this, SLOT(slot_read_socket()));

    socket->connectToHost("colossus", 5005);
}

void KioskElectricMeters::addMeter(QString name, QString label, MeterType type)
{
    KioskMeterWidget *meter = 0;

    switch(type) {
        case WATT_METER:
            meter = new KioskWattMeter();
            ((KioskWattMeter*)meter)->setLabel(label);
            ((KioskWattMeter*)meter)->setObjectName(name);
            break;
        case VOLT_METER:
            meter = new KioskVoltMeter();
            ((KioskVoltMeter*)meter)->setLabel(label);
            ((KioskVoltMeter*)meter)->setObjectName(name);
            break;
    };

    if (!meter)
        return;

    layout->addWidget(meter, last_row, last_column, Qt::AlignHCenter|Qt::AlignVCenter);

    meters[name] = meter;
    height_hint = meter->sizeHint().height() > height_hint ?
        meter->sizeHint().height() : height_hint;

    last_row++;

    if (last_row == 3) {
        last_row = 0;
        last_column++;
    }
}

void KioskElectricMeters::setMeter(QString name, int value)
{
    KioskMeterWidget *meter = meters[name];

    if (meter)
        meter->setMeterValue(value);
}

void KioskElectricMeters::resizeEvent(QResizeEvent *event)
{
    //QWidget::setFixedSize(layout->sizeHint());
    QWidget::setFixedHeight(height_hint * 3 + 20 + label->height());
    //QWidget::resizeEvent(event);
    label->setFixedWidth(QWidget::width());
}

void KioskElectricMeters::slot_read_socket(void)
{
    while(socket->canReadLine()) {
        QString line = socket->readLine().trimmed();

        if (!line.length())
            continue;

        QStringList list = line.split(":");

        QString name = list[0];
        QString v1 = list[1];
        QString v2 = list[2];

        double value;
        if (v1.compare("-"))
            value = v1.toDouble();
        else
            value = v2.toDouble();

        if (meters.contains(name))
            setMeter(name, value);
    }
}

void KioskElectricMeters::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    event->ignore();
}

void KioskElectricMeters::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    event->ignore();
}

void KioskElectricMeters::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    event->ignore();
}

void KioskElectricMeters::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setOpacity(0.1);

    painter.fillRect(event->rect(), Qt::black);

    QWidget::paintEvent(event);
}
