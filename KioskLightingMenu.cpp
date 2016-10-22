/* */

#include "KioskLightingMenu.h"

LightingMenu::LightingMenu(void)
    : KioskMenuWidget(),
    m_settings(new KioskSettings())
{
    KioskMenuWidget::setObjectName("Lighting Menu");

    row = 0;
    column = 0;

    socket = new QTcpSocket();

    connect(socket, SIGNAL(readyRead()), this, SLOT(slot_read_socket()));

    socket->connectToHost("pc6", 5001);
}

void LightingMenu::slot_read_socket(void)
{
    QString line;

    while(socket->canReadLine()) {
        line = socket->readLine().trimmed();

        QStringList list = line.split(":");

        QString dimmable;
        QString name;
        QString friendly;
        QString level;

        QString _type = list[0];
        LightType type = TYPE_LIGHT;

        if (!_type.compare("DEVICE"))
            type = TYPE_LIGHT;
        else if (!_type.compare("SCENE"))
            type = TYPE_SCENE;

        switch(type) {
            case TYPE_LIGHT:
                dimmable = list[1];
                name = list[2];
                friendly = list[3];
                level = list[4];
                break;
            case TYPE_SCENE:
                name = list[1];
                friendly = list[2];
                level = list[3];
                break;
        };

        qreal _level = level.toUInt();

        int min, max;
        m_settings->dimmerRange(name, min, max);
        int range = max - min;
        _level -= min;
        _level = (_level / range) * 100.0;

        if (!m_skipUpdate[name])
            check_light_button(dimmable.toUInt(), type, name, friendly, (uint)_level);

        m_skipUpdate[name] = false;
    }
}

void LightingMenu::check_light_button(bool dimmable, LightType type, QString name, QString friendly, uint level)
{
    if (column > 2) {
        row++;
        column = 0;
    }

    if (lights.contains(name)) {
        KioskSliderButton *button = lights.value(name);
        button->setLevel(level);
        //button->setEnabled(true);
    } else {
        KioskSliderButton *button = new KioskSliderButton(this, type, name,
          friendly, dimmable, level, this, SLOT(slot_light_schedule(const QString, const LightType)),
          SLOT(slot_light_slider(const QString, const LightType)));

        add_button(button, column, row);

        lights[name] = button;

        column++;
    }
}

void LightingMenu::slot_light_schedule(const QString name, const LightType type)
{
    if (lights.contains(name)) {
        KioskSliderButton *button = lights.value(name);
        KioskLightScheduleEditor *editor = new KioskLightScheduleEditor();
        editor->setupUI(button);

        connect(editor, SIGNAL(sliderChanged(const QString, const LightType)),
                this, SLOT(slot_light_slider(const QString, const LightType)));

        KioskMenuWidget::changeTo(editor);
    }
}

void LightingMenu::slot_light_slider(const QString name, const LightType type)
{
    QString command;
    QString state;

    if (lights.contains(name)) {
        KioskSliderButton *button = lights.value(name);

        int level = button->level();

        int min, max;
        m_settings->dimmerRange(button->name(), min, max);
        int range = max - min;
        level = (range * (level / 100.0)) + min;

        QString typeS;
        switch(type) {
            case TYPE_LIGHT:
                typeS = "DEVICE";
                break;
            case TYPE_SCENE:
                typeS = "SCENE";
                break;
        };

        if (!button->isDimmable()) {
            QString state = (level ? "on" : "off");
            command = QString("set %1.%2:%3\n").arg(typeS).arg(name).arg(state);
        } else
            command = QString("set %1.%2:level:%3:100\n").arg(typeS).arg(name).arg(level);

        if (socket->state() == QAbstractSocket::ConnectedState) {
            socket->write(qPrintable(command), command.length());
            socket->flush();
qDebug() << qPrintable(QString(command));
            m_skipUpdate[name] = true;
            //button->setEnabled(false);
        }
    }
}
