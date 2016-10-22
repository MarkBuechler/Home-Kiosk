/* */

#include "KioskJukeboxSelector.h"

KioskJukeboxSelectorButton::KioskJukeboxSelectorButton(QWidget *parent)
    : KioskButtonBase(parent)
{
    setType(ButtonTypeNumber);
    setOpacity(0.0);
}

void KioskJukeboxSelectorButton::setText(QString text)
{
    main_text = text;
    repaint();
}

void KioskJukeboxSelectorButton::setType(JukeboxButtonType type)
{
    m_type = type;

    switch(m_type) {
        case ButtonTypeLetter:
            m_background = "wbutton";
            QWidget::setFixedSize(80, 50);
            KioskButtonBase::setCheckable(true);
            break;
        case ButtonTypeNumber:
            m_background = "rbutton";
            QWidget::setFixedSize(80, 50);
            KioskButtonBase::setCheckable(true);
            break;
        case ButtonTypeReset:
            m_background = "reset";
            QWidget::setFixedSize(156, 63);
            KioskButtonBase::setCheckable(false);
            break;
        case ButtonTypeRandom:
            m_background = "random";
            QWidget::setFixedSize(156, 63);
            KioskButtonBase::setCheckable(false);
            break;
    }

    repaint();
}

void KioskJukeboxSelectorButton::paintEvent(QPaintEvent *event)
{
    KioskButtonBase::paintEvent(event);

    QPainter painter(this);

    QRect rect = QWidget::rect(); // whole thing

    QPixmap background;
    QColor fcolor;

    if (KioskButtonBase::isChecked() || KioskButtonBase::isPressed())
        background = QPixmap(QString(":/res/jukebox_%1_pushed.png").arg(m_background));
    else
        background = QPixmap(QString(":/res/jukebox_%1.png").arg(m_background));

    if (m_type == ButtonTypeLetter)
        fcolor = QColor("#a0121a");
    else
        fcolor = QColor("#f1f9bc");

    painter.drawPixmap(rect, background);

    float centerX = rect.width() / 2;
    float centerY = rect.height() / 2 + 1;

    if (KioskButtonBase::isChecked() || KioskButtonBase::isPressed()) {
        centerX += 2;
        centerY += 2;
    }

    int main_font_size = int(rect.width() * .20);

	QFont font;
	font.setPointSize(main_font_size);
	font.setBold(true);

	QPen text_pen;
	text_pen.setBrush(fcolor);

	painter.setPen(text_pen);
	painter.setFont(font);

	QSize main_size;

    if (!main_text.isEmpty())
        main_size = _textSize(&painter, main_text);

    QPointF main_center = _center(main_size, !main_text.isEmpty());

    float mainX = centerX - main_center.x();
    float mainY;

    if (!main_text.isEmpty())
        mainY = centerY + main_center.y();
    else
        mainY = centerY - main_center.y();

    main_point  = QPointF(mainX, mainY);

    if (!main_text.isEmpty())
        painter.drawText(main_point, main_text);
}

QSize KioskJukeboxSelectorButton::_textSize(QPainter *painter, QString text)
{
    return QSize(painter->fontMetrics().width(text, text.length()),
                 painter->fontMetrics().lineSpacing());
}

QPointF KioskJukeboxSelectorButton::_center(QSize size, bool is_text)
{
    float x = size.width() / 2;
    float y;

    if (is_text)
        y = size.height() / 4;
    else
        y = size.height() / 2;

    return QPointF(x, y);
}

KioskJukeboxWaitLight::KioskJukeboxWaitLight(void)
    : QWidget(),
    m_lit(false)
{
    QWidget::setFixedSize(156, 63);
}

void KioskJukeboxWaitLight::setLit(bool set)
{
    m_lit = set;
    repaint();
}

void KioskJukeboxWaitLight::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect rect = QWidget::rect();

    QPixmap light;

    if (m_lit)
        light = QPixmap(":/res/jukebox_wait_lit.png");
    else
        light = QPixmap(":/res/jukebox_wait.png");

    painter.drawPixmap(rect, light);
}

KioskJukeboxSelector::KioskJukeboxSelector(QWidget *parent)
    : QWidget(parent),
    m_letterPressed(false),
    m_numberPressed(false),
    m_busy(false)
{
    QWidget::setFixedSize(320, 700);

        QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(10);

    QHBoxLayout *top_layout = new QHBoxLayout();
    top_layout->setSpacing(20);
    top_layout->setMargin(0);

    main_layout->addLayout(top_layout);

    QVBoxLayout *column_layout;

    // Letters
    column_layout = new QVBoxLayout();
    column_layout->setMargin(0);
    column_layout->setSpacing(0);
    top_layout->addLayout(column_layout);

    QSignalMapper *lettermapper = new QSignalMapper(this);

    for (int l = 0; l < 10; l++) {
        letters[l] = new KioskJukeboxSelectorButton();
        column_layout->addWidget(letters[l]);
        letters[l]->setAutoRepeat(false);
        letters[l]->setType(ButtonTypeLetter);

        int label = l + 0x41;
        if (label >= 0x49)
            label++;

        letters[l]->setText(QChar(label));

        connect(letters[l], SIGNAL(released()), lettermapper, SLOT(map()));
        lettermapper->setMapping(letters[l], l);
    }

    // Numbers
    column_layout = new QVBoxLayout();
    column_layout->setMargin(0);
    column_layout->setSpacing(0);
    top_layout->addLayout(column_layout);

    QSignalMapper *numbermapper = new QSignalMapper(this);

    for (int l = 0; l < 20; l++) {
        if (l == 10) {
            column_layout = new QVBoxLayout();
            column_layout->setMargin(0);
            column_layout->setSpacing(0);
            top_layout->addLayout(column_layout);
        }

        numbers[l] = new KioskJukeboxSelectorButton();
        column_layout->addWidget(numbers[l]);
        numbers[l]->setAutoRepeat(false);
        numbers[l]->setText(QString("%1").arg(l + 1));

        connect(numbers[l], SIGNAL(released()), numbermapper, SLOT(map()));
        numbermapper->setMapping(numbers[l], l);
    }

    QHBoxLayout *bottom_layout = new QHBoxLayout();
    bottom_layout->setSpacing(20);

    m_waitLight = new KioskJukeboxWaitLight();

    m_resetButton = new KioskJukeboxSelectorButton();
    m_resetButton->setType(ButtonTypeReset);

    bottom_layout->addWidget(m_waitLight);
    bottom_layout->addWidget(m_resetButton);

    main_layout->addLayout(bottom_layout);

    m_randomButton = new KioskJukeboxSelectorButton();
    m_randomButton->setType(ButtonTypeRandom);

    main_layout->addWidget(m_randomButton, 0, Qt::AlignRight);

    connect(lettermapper, SIGNAL(mapped(int)), this, SLOT(slot_letter_press(int)));
    connect(numbermapper, SIGNAL(mapped(int)), this, SLOT(slot_number_press(int)));

    connect(m_resetButton, SIGNAL(clicked()), this, SLOT(_reset()));
    connect(m_randomButton, SIGNAL(clicked()), this, SLOT(slot_random()));
}

void KioskJukeboxSelector::reset(void)
{
    m_waitLight->setLit(true);
    QTimer::singleShot(5000, this, SLOT(_reset()));
}

void KioskJukeboxSelector::_reset(void)
{
    for (int x = 0; x < 10; x++) {
        letters[x]->setChecked(false);
    }
    for (int x = 0; x < 20; x++) {
        numbers[x]->setChecked(false);
    }

    m_letterPressed = m_numberPressed = false;

    m_waitLight->setLit(false);
}

void KioskJukeboxSelector::setBusy(bool busy)
{
    m_waitLight->setLit(busy);
    m_busy = busy;
#warning: todo: lock buttons on busy
}

void KioskJukeboxSelector::signalSelection(void)
{
    QString letter;
    QString number;

    for (int x = 0; x < 10; x++)
        if (letters[x]->isChecked())
            letter = letters[x]->text();
    for (int x = 0; x < 20; x++)
        if (numbers[x]->isChecked())
            number = numbers[x]->text();

    if (!m_busy)
        emit selectionMade(QString("%1%2").arg(letter).arg(number));
}

void KioskJukeboxSelector::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setOpacity(0.5);
    painter.fillRect(QWidget::rect(), Qt::black);
}

void KioskJukeboxSelector::slot_letter_press(int index)
{
    for (int x = 0; x < 10; x++)
        if (x != index)
            letters[x]->setChecked(false);

    m_letterPressed = true;

    if (m_letterPressed && m_numberPressed)
        signalSelection();
}

void KioskJukeboxSelector::slot_number_press(int index)
{
    for (int x = 0; x < 20; x++)
        if (x != index)
            numbers[x]->setChecked(false);

    m_numberPressed = true;

    if (m_letterPressed && m_numberPressed)
        signalSelection();
}

void KioskJukeboxSelector::slot_random(void)
{
    int number = 0;

    for (int x = 0; x < 20; x++)
        if (numbers[x]->isChecked())
            number = numbers[x]->text().toInt();

    if (!number) {
        _reset();
        return;
    }

    if (!m_busy)
        emit randomMade(number);
}
