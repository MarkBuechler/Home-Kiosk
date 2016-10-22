/* */

#include "KioskLightScheduleEditor.h"

KioskLightScheduleTab::KioskLightScheduleTab(QWidget *parent)
    : QWidget(parent),
    m_widget(0)
{
    m_layout = new QVBoxLayout(this);

    QHBoxLayout *menu_layout = new QHBoxLayout();

    m_enableBox = new KioskCheckbox();
    m_enableBox->setText("Enable Scheduling");
    m_enableBox->setCheckable(true);

    menu_layout->addWidget(m_enableBox);
    menu_layout->addStretch(1);

    m_layout->addLayout(menu_layout);

    connect(m_enableBox, SIGNAL(toggled()), this, SLOT(slot_enable_widget()));
}

void KioskLightScheduleTab::setUI(QWidget *uiWidget)
{
    m_layout->addWidget(uiWidget, 7);
    m_widget = uiWidget;
    m_widget->setEnabled(false);
}

void KioskLightScheduleTab::slot_enable_widget(void)
{
    if (m_widget)
        m_widget->setEnabled(m_enableBox->isChecked());
}

KioskManualSchedule::KioskManualSchedule(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *top = new QVBoxLayout(this);

    m_hourSlider = new KioskSlideWidget();
    m_hourSlider->setRange(0, 24, 0.25, 1);
    m_hourSlider->setDirection(NoGradient);
    m_hourSlider->setDualSliders(true);

    top->addWidget(m_hourSlider);

    QHBoxLayout *clockLayout = new QHBoxLayout();

    m_clock = new KioskClockWidget();

    clockLayout->addWidget(m_clock);

    top->addLayout(clockLayout);

    connect(m_hourSlider, SIGNAL(sliderMoved(SliderNumber)), this, SLOT(slot_adjust_clock()));
}

void KioskManualSchedule::slot_adjust_clock(void)
{
    QHash<int,int> hash;

    qreal start = m_hourSlider->value(SLIDER1);
    qreal end   = m_hourSlider->value(SLIDER2);

    start *= 60.0 * 60.0;
    end   *= 60.0 * 60.0;

    hash[start] = end;

    m_clock->setShaded(hash);
}

KioskSunSchedule::KioskSunSchedule(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *top = new QVBoxLayout(this);

    QGridLayout *icon_layout = new QGridLayout();

    QWidget *sunriseWidget = new QWidget();
    QHBoxLayout *sunriseLayout = new QHBoxLayout(sunriseWidget);
    m_sunrise = new KioskButton();
    m_sunrise->setIcon(QIcon(":/res/sched_sunrise.png"));
    m_sunrise->setIconSize(QSize(128, 128));
    m_sunrise->setCheckable(true);
    m_sunrise->setText("Sunrise");
    m_sunrise->setButtonStyle(TextUnderIcon);

    sunriseLayout->addWidget(m_sunrise);

    m_sunriseSwitch = new KioskLightSwitch();
    m_sunriseSwitch->setEnabled(false);
    sunriseLayout->addWidget(m_sunriseSwitch);

    QWidget *sunsetWidget = new QWidget();
    QHBoxLayout *sunsetLayout = new QHBoxLayout(sunsetWidget);
    m_sunset = new KioskButton();
    m_sunset->setIcon(QIcon(":/res/sched_sunset.png"));
    m_sunset->setIconSize(QSize(128, 128));
    m_sunset->setCheckable(true);
    m_sunset->setText("Sunset");
    m_sunset->setButtonStyle(TextUnderIcon);

    sunsetLayout->addWidget(m_sunset);

    m_sunsetSwitch = new KioskLightSwitch();
    m_sunsetSwitch->setEnabled(false);
    sunsetLayout->addWidget(m_sunsetSwitch);

    icon_layout->addWidget(sunriseWidget, 0, 0, Qt::AlignHCenter|Qt::AlignBottom);
    icon_layout->addWidget(sunsetWidget, 0, 1, Qt::AlignHCenter|Qt::AlignBottom);

    m_sunriseSlider = new KioskSlideWidget();
    m_sunriseSlider->setRange(-4, 4, 0.5);
    m_sunriseSlider->setDirection(DarkToLight);
    m_sunriseSlider->setEnabled(false);

    m_sunsetSlider = new KioskSlideWidget();
    m_sunsetSlider->setRange(-4, 4, 0.5);
    m_sunsetSlider->setDirection(LightToDark);
    m_sunsetSlider->setEnabled(false);

    icon_layout->addWidget(m_sunriseSlider, 1, 0, Qt::AlignHCenter|Qt::AlignTop);
    icon_layout->addWidget(m_sunsetSlider, 1, 1, Qt::AlignHCenter|Qt::AlignTop);

    top->addLayout(icon_layout);

    connect(m_sunrise, SIGNAL(toggled()), this, SLOT(slot_sunrise()));
    connect(m_sunset, SIGNAL(toggled()), this, SLOT(slot_sunset()));
}

void KioskSunSchedule::resizeEvent(QResizeEvent *event)
{
    m_sunriseSlider->setFixedWidth(m_sunrise->size().width() * 1.5);
    m_sunsetSlider->setFixedWidth(m_sunset->size().width() * 1.5);
}

void KioskSunSchedule::slot_sunrise(void)
{
    m_sunriseSlider->setEnabled(m_sunrise->isChecked());
    m_sunriseSwitch->setEnabled(m_sunrise->isChecked());
}

void KioskSunSchedule::slot_sunset(void)
{
    m_sunsetSlider->setEnabled(m_sunset->isChecked());
    m_sunsetSwitch->setEnabled(m_sunset->isChecked());
}

KioskDurationSchedule::KioskDurationSchedule(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *top = new QVBoxLayout(this);

    m_durationSlider = new KioskSlideWidget();
    m_durationSlider->setRange(0, 24, 0.25);
    m_durationSlider->setDirection(NoGradient);

    top->addWidget(m_durationSlider);
}

KioskLightScheduleEditor::KioskLightScheduleEditor(QWidget *parent)
    : KioskWidget(),
    m_button(0),
    m_status(0),
    m_settings(new KioskSettings())
{
    KioskWidget::setObjectName("Lighting Scheduler");
    KioskWidget::setUsesKeyboard(false);
}

void KioskLightScheduleEditor::setupUI(KioskSliderButton *button)
{
    m_button = button;

    QVBoxLayout *top = new QVBoxLayout(this);

    QHBoxLayout *statusLayout = new QHBoxLayout();

    top->addLayout(statusLayout, 1);

    m_status = new KioskLightStatus();

    statusLayout->addWidget(m_status);

    if (button->isDimmable()) {
        m_slider = new KioskLightDimmer();
        ((KioskLightDimmer*)m_slider)->setDimmer(button->level());
    } else {
        m_slider = new KioskLightSwitch();
        ((KioskLightSwitch*)m_slider)->setChecked(button->level(), false);
    }

    statusLayout->addWidget(m_slider);

    if (button->isDimmable()) {
        m_adjuster = new KioskSlideWidget();
        m_adjuster->setRange(0, 100, 1, 10);
        m_adjuster->setDualSliders(true);
        m_adjuster->setGradientFollowsValue(true);
        m_adjuster->setGradientStartColor(QColor(255, 255, 100, 255));
        m_adjuster->setGradientEndColor(Qt::white);

        int min, max;
        m_settings->dimmerRange(button->name(), min, max);
        m_adjuster->setValue(SLIDER1, min);
        m_adjuster->setValue(SLIDER2, max);

        statusLayout->addWidget(m_adjuster, 1);
    } else
        statusLayout->addStretch(1);

    m_status->setLevel(button->level());

    slot_slider_changed(button->level());

    tabs = new QTabWidget();

    top->addWidget(tabs, 7);

    m_sunSchedule = new KioskLightScheduleTab();
    m_sunSchedule->setUI(new KioskSunSchedule());

    m_manualSchedule = new KioskLightScheduleTab();
    m_manualSchedule->setUI(new KioskManualSchedule());

    m_durationSchedule = new KioskLightScheduleTab();
    m_durationSchedule->setUI(new KioskDurationSchedule());

    tabs->addTab(m_sunSchedule, "Sunrise && Sunset");
    tabs->addTab(m_manualSchedule, "Manual Scheduling");
    tabs->addTab(m_durationSchedule, "Durations");

    if (button->isDimmable()) {
        connect(m_slider, SIGNAL(sliderMoved(int)), this, SLOT(slot_slider_moved(int)));
        connect(m_slider, SIGNAL(sliderChanged(int)), this, SLOT(slot_slider_changed(int)));
        connect(m_adjuster, SIGNAL(valueChanged(SliderNumber, int)), this, SLOT(slot_adjuster_changed(SliderNumber, int)));
    } else
        connect(m_slider, SIGNAL(clicked()), this, SLOT(slot_slider_changed()));
}

void KioskLightScheduleEditor::saveDimmerRanges(void)
{
    int min = m_adjuster->value(SLIDER1);
    int max = m_adjuster->value(SLIDER2);

    m_settings->setDimmerRange(m_button->name(), min, max);
}

void KioskLightScheduleEditor::slot_adjuster_changed(SliderNumber, int)
{
    saveDimmerRanges();
    slot_slider_changed(((KioskLightDimmer*)m_slider)->dimmer());
}

void KioskLightScheduleEditor::slot_slider_moved(int value)
{
    m_status->setLevel(value);
}

void KioskLightScheduleEditor::slot_slider_changed(int value)
{
    if (m_button) {
        if (value == -1)
            value = ((KioskLightSwitch*)m_slider)->isChecked() ? 100 : 0;

        m_button->setLevel(value);
        m_status->setLevel(value);

        emit sliderChanged(m_button->name(), m_button->type());
    }
}
