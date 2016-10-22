/* */

#include "KioskGUI.h"
#include "KioskWebBrowser.h"
#include "KioskDebug.h"

KioskClock::KioskClock(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(0);

    // Draws garbage on the background if we use gl
    //QGLFormat format = QGLFormat::defaultFormat();
    //format.setSampleBuffers(false);
    //QGLWidget *glWidget = new QGLWidget(format);

    QDeclarativeView *qmlView = new QDeclarativeView();

    //glWidget->setAutoFillBackground(false);
    //qmlView->setViewport(glWidget);

    qmlView->viewport()->setAutoFillBackground(false);
    qmlView->setSource(QUrl("qrc:/clocks.qml"));
    qmlView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    QObject *item = qmlView->rootObject();

    vbox->addWidget(qmlView);
}

KioskGraphicsView::KioskGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    QWidget *widget = QGraphicsView::viewport();
    //grabGesture(QtScroller::LeftMouseButtonGesture);
    QFrame::setFrameShape(QFrame::NoFrame);

    m_scroller = QtScroller::scroller(widget);

    //QtScrollerProperties properties = m_scroller->scrollerProperties();
    //properties.setScrollMetric(QtScrollerProperties::DragStartDistance, 10);
    //m_scroller->setScrollerProperties(properties);
}

void KioskGraphicsView::grabGesture(QtScroller::ScrollerGestureType type)
{
    QWidget *widget = QGraphicsView::viewport();
    QtScroller::grabGesture(widget, type);
}

void KioskGraphicsView::ungrabGesture(void)
{
    QWidget *widget = QGraphicsView::viewport();
    QtScroller::ungrabGesture(widget);
}

void KioskGraphicsView::setOvershoot(bool overshoot)
{
    QtScrollerProperties properties = m_scroller->scrollerProperties();

    if (overshoot) {
        properties.setHorizontalOvershootPolicy(QtScrollerProperties::OvershootAlwaysOn);
        properties.setVerticalOvershootPolicy(QtScrollerProperties::OvershootAlwaysOn);
    } else {
        properties.setHorizontalOvershootPolicy(QtScrollerProperties::OvershootAlwaysOff);
        properties.setVerticalOvershootPolicy(QtScrollerProperties::OvershootAlwaysOff);
    }

    m_scroller->setScrollerProperties(properties);
}

KioskDeclarativeView::KioskDeclarativeView(QWidget *parent)
    : QDeclarativeView(parent)
{
}

QObject *KioskDeclarativeView::setQmlItem(KioskQmlItem *item)
{
    m_item = item;

    QDeclarativeView::setObjectName(item->name());

    if (item->usesGL()) {
        QGLFormat format = QGLFormat::defaultFormat();
        format.setSampleBuffers(false);
        QGLWidget *glWidget = new QGLWidget(format);
        QDeclarativeView::setViewport(glWidget);
    }

    if (item->transparent())
        QDeclarativeView::viewport()->setAutoFillBackground(false);

    QDeclarativeView::setSource(item->source());
    QDeclarativeView::setResizeMode(QDeclarativeView::SizeRootObjectToView);

    return QDeclarativeView::rootObject();
}

void KioskDeclarativeView::reset(void)
{
    QDeclarativeView::setSource(QUrl(""));
    m_item = 0;
}

KioskScrollArea::KioskScrollArea(QWidget *parent)
    : QScrollArea(parent)
{
    QWidget *widget = QAbstractScrollArea::viewport();
    grabGesture(QtScroller::LeftMouseButtonGesture);
    QFrame::setFrameShape(QFrame::NoFrame);

    m_scroller = QtScroller::scroller(widget);
}

void KioskScrollArea::grabGesture(QtScroller::ScrollerGestureType type)
{
    QWidget *widget = QAbstractScrollArea::viewport();
    QtScroller::grabGesture(widget, type);
}

void KioskScrollArea::ungrabGesture(void)
{
    QWidget *widget = QAbstractScrollArea::viewport();
    QtScroller::ungrabGesture(widget);
}

void KioskScrollArea::setOvershoot(bool overshoot)
{
    QtScrollerProperties properties = m_scroller->scrollerProperties();

    if (overshoot) {
        properties.setHorizontalOvershootPolicy(QtScrollerProperties::OvershootAlwaysOn);
        properties.setVerticalOvershootPolicy(QtScrollerProperties::OvershootAlwaysOn);
    } else {
        properties.setHorizontalOvershootPolicy(QtScrollerProperties::OvershootAlwaysOff);
        properties.setVerticalOvershootPolicy(QtScrollerProperties::OvershootAlwaysOff);
    }

    m_scroller->setScrollerProperties(properties);
}

KioskSliderButton::KioskSliderButton(QWidget *parent, LightType _type, QString _name, QString text,
  bool _dimmable, int initial_value, QObject *caller, const char *button_slot, const char *slider_slot)
    : QWidget(parent),
    m_enabled(true)
{
    m_top = new QVBoxLayout(this);
    m_top->setSizeConstraint(QLayout::SetMinimumSize);

    m_name = _name;
    m_type = _type;
    m_dimmable = _dimmable;

    KioskHeaderLabel *label = new KioskHeaderLabel();
    label->setText(text);

    m_top->addWidget(label);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(20);

    m_button = new KioskButton();

    if (m_dimmable)
        m_slider = new KioskLightDimmer();
    else
        m_slider = new KioskLightSwitch();

    m_light = new KioskLightStatus();

    setLevel(initial_value);

    m_button->setWidget(m_light);

    m_button->setObjectName(_name);

    layout->addStretch(1);
    layout->addWidget(m_button);
    layout->addWidget(m_slider);
    layout->addStretch(1);

    m_top->addLayout(layout, 7);

    connect(m_button, SIGNAL(clicked()), this, SLOT(slot_button_clicked()));

    if (m_dimmable) {
        connect(m_slider, SIGNAL(sliderMoved(int)), this, SLOT(slot_slider_moved(int)));
        connect(m_slider, SIGNAL(sliderChanged(int)), this, SLOT(slot_slider_changed()));
    } else
        connect(m_slider, SIGNAL(clicked()), this, SLOT(slot_slider_changed()));

    connect(this, SIGNAL(buttonClicked(const QString, const LightType)), caller, button_slot);
    connect(this, SIGNAL(sliderChanged(const QString, const LightType)), caller, slider_slot);
}

void KioskSliderButton::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();
    size.setWidth(m_button->width() + m_slider->width() + 10);
    size.setHeight(m_button->height() + 4);

    m_slider->resize(m_slider->width(), m_button->height());

    QWidget::resizeEvent(event);
}

void KioskSliderButton::paintEvent(QPaintEvent *event)
{
    QWidget::setFixedSize(m_top->sizeHint());

    QPainter painter(this);
    painter.setOpacity(0.3);

    QRect rect = QWidget::rect();
    rect.adjust(0, 0, -1, -1); // prevent bottom and right from getting cut

    QPainterPath path;
    path.addRoundedRect(rect, 14, 14);
    painter.setBrush(Qt::black);
    painter.drawPath(path);
}

void KioskSliderButton::setEnabled(bool enable)
{
    m_button->setEnabled(enable);
    m_slider->setEnabled(enable);
    m_enabled = enable;
}

int KioskSliderButton::level(void)
{
    if (m_dimmable)
        return ((KioskLightDimmer*)m_slider)->dimmer();
    else
        return ((KioskLightSwitch*)m_slider)->isChecked() ? 100 : 0;
}

void KioskSliderButton::setLevel(int _level)
{
    m_light->setLevel(_level);

    if (m_dimmable)
        ((KioskLightDimmer*)m_slider)->setDimmer(_level);
    else
        ((KioskLightSwitch*)m_slider)->setChecked(_level, false);
}

void KioskSliderButton::slot_button_clicked(void)
{
    emit buttonClicked(m_name, m_type);
}

void KioskSliderButton::slot_slider_moved(int value)
{
    m_light->setLevel(value);
}

void KioskSliderButton::slot_slider_changed(void)
{
    if (!m_dimmable) {
        int value = ((KioskLightSwitch*)m_slider)->isChecked() ? 100 : 0;
        m_light->setLevel(value);
    }
    emit sliderChanged(m_name, m_type);
}

KioskMenuButton::KioskMenuButton(QWidget *parent,
  QString image, QString text, QObject *caller, const char *slot)
    : KioskButton(parent)
{
    KioskButton::setIcon(QIcon(image));
    KioskButton::setIconSize(QSize(128, 128));
    KioskButton::setText(text);
    KioskButton::setButtonStyle(TextUnderIcon);

    connect(this, SIGNAL(clicked()), caller, slot);
}

KioskToolButton::KioskToolButton(QWidget *parent,
  QString image, QString text, QObject *caller, const char *slot)
    : KioskButton(parent)
{
    //KioskButton::setMargin(5);
    KioskButton::setIcon(QIcon(image));
    KioskButton::setIconSize(QSize(32, 32));
    KioskButton::setText(text);
    KioskButton::setButtonStyle(TextBesideIcon);

    connect(this, SIGNAL(clicked()), caller, slot);
}

KioskWidget::KioskWidget(KioskDBLink *link) : QWidget()
{
    uses_keyboard = true;
    uses_notepad = true;
    is_status = false;
    m_db = link;
}

void KioskWidget::changeTo(QWidget *widget)
{
    emit changeKiosk(widget);
}

void KioskWidget::changeToBrowser(QUrl url)
{
    emit changeBrowser(url);
}

void KioskWidget::changeToQml(KioskQmlItem *item)
{
    emit changeQml(item);
}

KioskMenuWidget::KioskMenuWidget(KioskDBLink *link)
    : KioskWidget(link)
{
    KioskWidget::setUsesKeyboard(false);

    //QWidget::setWindowOpacity(0.85);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(0);

    QWidget *menu = new QWidget();
    menu_layout = new QGridLayout(menu);
    menu_layout->setSpacing(20);

    vbox->addWidget(menu, 1);
}

void KioskMenuWidget::add_button(QWidget *button, int row, int col)
{
    menu_layout->addWidget(button, row, col);
}

void KioskMenuWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setOpacity(0.1);

    painter.fillRect(event->rect(), Qt::black);

    QWidget::paintEvent(event);
}

KioskStyle::KioskStyle(void)
{
}

int KioskStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    if (metric == QStyle::PM_ScrollBarExtent) // && widget == 0
        return 50;
    else if (metric == QStyle::PM_SliderThickness)
        return 40;
    else if (metric == QStyle::PM_SliderLength)
        return 40;

    return QGtkStyle::pixelMetric(metric, option, widget);
}

KioskToolbar::KioskToolbar(QWidget *parent) : QWidget(parent)
{
    toolbar_layout = new QHBoxLayout(this);
    toolbar_layout->setMargin(0);

    previous_button = new KioskToolButton(this, ":/res/back.png", "", this, SLOT(slot_previous()));

    if(parent)
        previous_button->setText(parent->windowTitle());

    toolbar_layout->addStretch(1);
    toolbar_layout->addWidget(previous_button);
}

void KioskToolbar::insertWidget(int index, QWidget *widget, int stretch, Qt::Alignment alignment)
{
    toolbar_layout->insertWidget(index, widget, stretch, alignment);
}

void KioskToolbar::insertSpacerItem(int index, QSpacerItem *spacerItem)
{
    toolbar_layout->insertSpacerItem(index, spacerItem);
}

void KioskToolbar::slot_previous(void)
{
    emit previousWindow();
}

KioskHeaderLabel::KioskHeaderLabel(QWidget *parent)
    : QLabel(parent)
{
    QFont font;
    font.setPointSize(16);
    font.setBold(true);

    QPalette palette = QLabel::palette();
    palette.setColor(QPalette::Foreground, QColor("#eeeeee"));
    QLabel::setPalette(palette);
    QLabel::setFont(font);
    QLabel::setAlignment(Qt::AlignHCenter);
    QLabel::setMargin(3);
}

void KioskHeaderLabel::paintEvent(QPaintEvent *event)
{
#warning: this is broken for graphicsview
    //QPainter painter(this);
    //painter.setOpacity(0.2);

    //painter.fillRect(event->rect(), Qt::black);

    QLabel::paintEvent(event);
}

KioskLightStatus::KioskLightStatus(QWidget *parent)
    : QWidget(parent),
    m_level(0)
{
    QWidget::setFixedSize(128, 128);
}

void KioskLightStatus::setLevel(int _level)
{
    m_level = _level;
    repaint();
}

void KioskLightStatus::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pixmap(":/res/light_var.png");
    QRect rect = pixmap.rect();

    uint centerX = rect.width() / 2;
    uint centerY = rect.height() / 2;

    QRadialGradient gradient(centerX+1, 42, 43, centerX+1, 42);

    double transparency = (m_level / 100.0) * 255.0;
    QColor yellowC(255, 255, 100, transparency);
    QColor whiteC(255, 255, 255, transparency);

    float pos = m_level / 100.0 * 0.9;
    float pos2 = pos - 0.01;
    if (pos2 < 0.0) pos2 = 0.0;

    gradient.setColorAt(0.0, whiteC);
    gradient.setColorAt(pos2, whiteC);
    gradient.setColorAt(pos, yellowC);
    //gradient.setColorAt(0.45, startC);
    gradient.setColorAt(1.0, QColor(255, 255, 255, 0));
    gradient.setSpread(QGradient::PadSpread);

    painter.setBrush(gradient);
    painter.setPen(Qt::transparent);
    painter.drawRect(QWidget::rect());

    painter.drawPixmap(QPoint(0, 0), pixmap, QWidget::rect());
}

