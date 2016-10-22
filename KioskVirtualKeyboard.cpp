/* */

#include "KioskVirtualKeyboard.h"

KioskKeyboardButton::KioskKeyboardButton(QWidget *parent)
    : KioskButtonBase(parent)
{
    invalidate = true;
}

void KioskKeyboardButton::setText(QString text)
{
    invalidate = true;
    main_text = text;
    repaint();
}

void KioskKeyboardButton::setSuperscriptText(QString text)
{
    invalidate = true;
    superscript_text = text;
    repaint();
}

void KioskKeyboardButton::setIcon(QIcon icon)
{
    invalidate = true;
    main_text = "";
    main_icon = icon;
    repaint();
}

void KioskKeyboardButton::setIconSize(QSize size)
{
    main_icon_size = size;
    repaint();
}

void KioskKeyboardButton::setSuperscriptIcon(QIcon icon)
{
    invalidate = true;
    superscript_text = "";
    superscript_icon = icon;
    repaint();
}

void KioskKeyboardButton::setSuperscriptIconSize(QSize size)
{
    superscript_icon_size = size;
    repaint();
}

void KioskKeyboardButton::resizeEvent(QResizeEvent *event)
{
    invalidate = true;
    QWidget::resizeEvent(event);
}

void KioskKeyboardButton::paintEvent(QPaintEvent *event)
{
    KioskButtonBase::paintEvent(event);

    QPainter painter(this);

    QRect rect = QWidget::rect(); // whole thing
    float centerX = rect.width() / 2;
    float centerY = rect.height() / 2;

    int main_font_size = int(rect.width() * .25);
    int super_font_size = int(main_font_size / 2);

	QFont font;
	font.setPointSize(main_font_size);
	font.setBold(true);

	QPen text_pen;
	text_pen.setBrush(Qt::white);

	painter.setPen(text_pen);
	painter.setFont(font);

	QPixmap main_pixmap;
	QPixmap super_pixmap;
	QSize main_size;

    if (!main_icon.isNull())
        main_pixmap = _getPixmap(main_icon, main_icon_size);

    if (!superscript_icon.isNull())
        super_pixmap = _getPixmap(superscript_icon, superscript_icon_size);

    if (invalidate) {
        if (!main_text.isEmpty())
            main_size = _textSize(&painter, main_text);
        else if (!main_icon.isNull())
            main_size = main_pixmap.size();

        QPointF main_center = _center(main_size, !main_text.isEmpty());

        float mainX = centerX - main_center.x();
        float mainY;

        if (!main_text.isEmpty())
            mainY = centerY + main_center.y();
        else
            mainY = centerY - main_center.y();

        float superX = mainX / 2;
        float superY = mainY / 2;

        main_point  = QPointF(mainX, mainY);
        super_point = QPointF(superX, superY);

        invalidate = false;
    }

    if (!main_text.isEmpty())
        painter.drawText(main_point, main_text);
    else if (!main_icon.isNull())
        painter.drawPixmap(main_point, main_pixmap);

    if (!superscript_text.isEmpty()) {
        font.setPointSize(super_font_size);
        painter.setFont(font);
        painter.drawText(super_point, superscript_text);
    } else if (!superscript_icon.isNull())
        painter.drawPixmap(super_point, super_pixmap);
}

QPixmap KioskKeyboardButton::_getPixmap(QIcon icon, QSize size)
{
    QPixmap pixmap;

    if (size.isNull()) {
        QList<QSize> available = icon.availableSizes();
        size = available[0];
    }

    if (KioskButtonBase::isEnabled()) {
        if (KioskButtonBase::isPressed())
            pixmap = icon.pixmap(size, QIcon::Active, QIcon::On);
        else
            pixmap = icon.pixmap(size, QIcon::Normal, QIcon::On);
    } else
        pixmap = icon.pixmap(size, QIcon::Disabled, QIcon::Off);

    return pixmap;
}

QSize KioskKeyboardButton::_textSize(QPainter *painter, QString text)
{
    return QSize(painter->fontMetrics().width(text, text.length()),
                 painter->fontMetrics().lineSpacing());
}

QPointF KioskKeyboardButton::_center(QSize size, bool is_text)
{
    float x = size.width() / 2;
    float y;

    if (is_text)
        y = size.height() / 4;
    else
        y = size.height() / 2;

    return QPointF(x, y);
}

KioskVirtualKeyboard::KioskVirtualKeyboard(QWidget *parent)
    : QWidget(parent)
{
    vkb_parent = parent;

    is_shifted = false;
    is_alpha   = true;
    is_numeric = false;
    is_caps    = false;

    setupUI();
}

void KioskVirtualKeyboard::reset(void)
{
    buttons[33]->setChecked(false);
    buttons[35]->setChecked(false);
    buttons[34]->setChecked(true);
}

void KioskVirtualKeyboard::setupUI(void)
{
    QVBoxLayout *top_layout = new QVBoxLayout(this);
    top_layout->setMargin(0);

    for (int i = 0; i <= 42; i++) {
        buttons[i] = new KioskKeyboardButton();
    }

    QHBoxLayout *row_layout;

    for (int r = 0; r <= 3; r++) {
        row_layout = new QHBoxLayout();
        top_layout->addLayout(row_layout);

        if (!r || (r == 2))
            row_layout->addSpacerItem(new QSpacerItem(30, 1));

        for (int i = 0; i <= 10; i++) {
            int stretch = 3;
            if ((r == 3) && (i == 4)) // space
                stretch = 7;
            else if ((r == 1) && (i == 10)) // enter
                stretch = 4;

            row_layout->addWidget(buttons[(r*11) + i], stretch);
            if (((r * 11) + i) == 42) {
                row_layout->addSpacerItem(new QSpacerItem(30, 1));
                i = 10;
            }
        }
    }

    setButtonIcon(buttons[10], QIcon(":/res/backspace_50x70.png"), QSize(70, 50));
    setButtonIcon(buttons[21], QIcon(":/res/enter_50x70.png"), QSize(70, 50));
    setButtonIcon(buttons[33], QIcon(":/res/shift_50x50.png"), QSize(50, 50));
    setButtonIcon(buttons[41], QIcon(":/res/ok_50x50.png"), QSize(50, 50));
    setButtonIcon(buttons[42], QIcon(":/res/delete_50x50.png"), QSize(50, 50));

    buttons[10]->setAutoRepeat(true);
    buttons[11]->setText("caps");
    buttons[11]->setCheckable(true);
    buttons[21]->setButtonType(ButtonTypeOkClose);
    buttons[33]->setCheckable(true);
    buttons[34]->setText("abc");
    buttons[34]->setMutual(true);
    buttons[34]->setCheckable(true);
    buttons[34]->setChecked(true);
    buttons[35]->setText("123@");
    buttons[35]->setMutual(true);
    buttons[35]->setCheckable(true);
    buttons[36]->setText(".com");
    buttons[41]->setButtonType(ButtonTypeOkClose);

    connect(buttons[10], SIGNAL(pressed()), this, SLOT(slot_backspace_button()));
    connect(buttons[11], SIGNAL(toggled()), this, SLOT(slot_caps_button()));
    connect(buttons[21], SIGNAL(clicked()), this, SLOT(slot_return_button()));
    connect(buttons[33], SIGNAL(toggled()), this, SLOT(slot_shift_button()));
    connect(buttons[34], SIGNAL(toggled()), this, SLOT(slot_alpha_button()));
    connect(buttons[35], SIGNAL(toggled()), this, SLOT(slot_numeric_button()));
    connect(buttons[36], SIGNAL(clicked()), this, SLOT(slot_dotcom_button()));
    connect(buttons[38], SIGNAL(clicked()), this, SLOT(slot_left_button()));
    connect(buttons[39], SIGNAL(clicked()), this, SLOT(slot_right_button()));
    connect(buttons[41], SIGNAL(clicked()), this, SLOT(slot_ok_button()));
    connect(buttons[42], SIGNAL(clicked()), this, SLOT(slot_clear_button()));

    QSignalMapper *pressmapper = new QSignalMapper(this);

    for (int i = 0; i< buttons.count(); i++) {
        if (keys[i][0] != 0xff) {
            buttons[i]->setAutoRepeat(true);
            connect(buttons[i], SIGNAL(pressed()), pressmapper, SLOT(map()));
            pressmapper->setMapping(buttons[i], i);
        }
    }

    QSignalMapper *releasemapper = new QSignalMapper(this);

    for (int i = 0; i< buttons.count(); i++) {
        if (keys[i][0] != 0xff) {
            buttons[i]->setAutoRepeat(true);
            connect(buttons[i], SIGNAL(released()), releasemapper, SLOT(map()));
            releasemapper->setMapping(buttons[i], i);
        }
    }


    connect(pressmapper, SIGNAL(mapped(int)), this, SLOT(slot_button_press(int)));
    connect(releasemapper, SIGNAL(mapped(int)), this, SLOT(slot_button_release(int)));

    setupKeys();
}

void KioskVirtualKeyboard::setupKeys(void)
{
    int x = is_numeric * 2;
    x += is_shifted;
    int y = (is_shifted ? (x - 1) : (x + 1));

    for (int i = 0; i <= 42; i++) {
        if (keys[i][0] == 0xff) // skip
            continue;
        if (keys[i][x]) {
            buttons[i]->setText(QChar(keys[i][x]));
            if (keys[i][y] && (keys[i][x] != keys[i][y]))
                buttons[i]->setSuperscriptText(QChar(keys[i][y]));
            else
                buttons[i]->setSuperscriptText(QString());
        } else {
            if (!keys[i][x])
                buttons[i]->setText(QString());
            if (!keys[i][y])
                buttons[i]->setSuperscriptText(QString());
        }
    }

    if (is_shifted) {
        setButtonIcon(buttons[38], QIcon(":/res/arrow_up_50x50.png"), QSize(40, 40));
        setButtonIcon(buttons[39], QIcon(":/res/arrow_down_50x50.png"), QSize(40, 40));
        setButtonIcon(buttons[38], QIcon(":/res/arrow_left_50x50.png"), QSize(15, 15), true);
        setButtonIcon(buttons[39], QIcon(":/res/arrow_right_50x50.png"), QSize(15, 15), true);
    } else {
        setButtonIcon(buttons[38], QIcon(":/res/arrow_left_50x50.png"), QSize(40, 40));
        setButtonIcon(buttons[39], QIcon(":/res/arrow_right_50x50.png"), QSize(40, 40));
        setButtonIcon(buttons[38], QIcon(":/res/arrow_up_50x50.png"), QSize(15, 15), true);
        setButtonIcon(buttons[39], QIcon(":/res/arrow_down_50x50.png"), QSize(15, 15), true);
    }
}

void KioskVirtualKeyboard::setButtonIcon(KioskKeyboardButton *button, QIcon icon, QSize size, bool superscript)
{
    if (!superscript) {
        button->setIcon(icon);
        button->setIconSize(size);
    } else {
        button->setSuperscriptIcon(icon);
        button->setSuperscriptIconSize(size);
    }
}

void KioskVirtualKeyboard::sendEvent(Qt::KeyboardModifiers modifier, int key, QString c, int numkeys)
{
    if (vkb_parent && vkb_parent->focusWidget()) {
        QKeyEvent keyEvent(QEvent::KeyPress, key, modifier, c, false, numkeys);
        QApplication::sendEvent(vkb_parent->focusWidget(), &keyEvent);
    }
}

void KioskVirtualKeyboard::slot_caps_button(void)
{
    if (!is_caps && is_shifted)
        is_caps = true;
    else {
        is_shifted = !is_shifted;
        is_caps = !is_caps;
    }

    setupKeys();
}

void KioskVirtualKeyboard::slot_shift_button(void)
{
    if (is_caps)
        return;

    is_shifted = !is_shifted;

    setupKeys();
}

void KioskVirtualKeyboard::slot_alpha_button(void)
{
    if (is_alpha)
        return;

    buttons[35]->setChecked(false);

    is_alpha = true;
    is_numeric = !is_alpha;

    if (is_shifted)
        buttons[33]->setChecked(false);

    setupKeys();
}

void KioskVirtualKeyboard::slot_numeric_button(void)
{
    if (is_numeric)
        return;

    buttons[34]->setChecked(false);

    is_numeric = true;
    is_alpha = !is_numeric;

    if (is_shifted)
        buttons[33]->setChecked(false);

    setupKeys();
}

void KioskVirtualKeyboard::slot_left_button(void)
{
    if (is_shifted)
        sendEvent(Qt::NoModifier, Qt::Key_Up, "", 1);
    else
        sendEvent(Qt::NoModifier, Qt::Key_Left, "", 1);
}

void KioskVirtualKeyboard::slot_right_button(void)
{
    if (is_shifted)
        sendEvent(Qt::NoModifier, Qt::Key_Down, "", 1);
    else
        sendEvent(Qt::NoModifier, Qt::Key_Right, "", 1);
}

void KioskVirtualKeyboard::slot_ok_button(void)
{
    sendEvent(Qt::NoModifier, Qt::Key_Enter, "", 1);
    emit requestHide();
}

void KioskVirtualKeyboard::slot_clear_button(void)
{
    sendEvent(Qt::ControlModifier, Qt::Key_A, "", 2);
    sendEvent(Qt::NoModifier, Qt::Key_Backspace, "", 1);
}

void KioskVirtualKeyboard::slot_backspace_button(void)
{
    sendEvent(Qt::NoModifier, Qt::Key_Backspace, "", 1);
}

void KioskVirtualKeyboard::slot_return_button(void)
{
    sendEvent(Qt::NoModifier, Qt::Key_Enter, "", 1);
}

void KioskVirtualKeyboard::slot_dotcom_button(void)
{
    sendEvent(Qt::NoModifier, 0, ".com", 1);
}

void KioskVirtualKeyboard::slot_button_press(int index)
{
    int x = is_numeric * 2;
    x += is_shifted;

    Qt::KeyboardModifiers modifier = Qt::NoModifier;

    int key = 0;
    int numkeys = 1;

    if (is_shifted) {
        numkeys++;
        modifier |= Qt::ShiftModifier;
    }

    QString character(keys[index][x]);

    if (!character.isEmpty())
        sendEvent(modifier, key, character, numkeys);
#if 0
    if (is_numeric) {
        slot_alpha_button();
        buttons[34]->setChecked(true);
    }
#endif
}

void KioskVirtualKeyboard::slot_button_release(int index)
{
    if (is_shifted)
        buttons[33]->setChecked(false);
}

