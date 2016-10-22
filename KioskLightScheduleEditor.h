/* */

#ifndef _KIOSK_LIGHTSCHEDULEEDITOR_H_
#define _KIOSK_LIGHTSCHEDULEEDITOR_H_

#include <QtGui>

#include "KioskGUI.h"
#include "KioskCheckbox.h"
#include "KioskSlideWidget.h"
#include "KioskClockWidget.h"
#include "KioskSettings.h"

class KioskLightScheduleTab : public QWidget
{
    Q_OBJECT
public:
    KioskLightScheduleTab(QWidget *parent=0);

    void setUI(QWidget*);

private slots:
    void slot_enable_widget(void);

private:
    QVBoxLayout *m_layout;
    QWidget *m_widget;

    KioskCheckbox *m_enableBox;
};

class KioskManualSchedule : public QWidget
{
    Q_OBJECT
public:
    KioskManualSchedule(QWidget *parent=0);

private slots:
    void slot_adjust_clock(void);

private:
    KioskSlideWidget *m_hourSlider;
    KioskClockWidget *m_clock;
};

class KioskSunSchedule : public QWidget
{
    Q_OBJECT
public:
    KioskSunSchedule(QWidget *parent=0);

protected slots:
    void resizeEvent(QResizeEvent *event);

private slots:
    void slot_sunrise(void);
    void slot_sunset(void);

private:
    KioskButton *m_sunrise;
    KioskButton *m_sunset;

    KioskSlideWidget *m_sunriseSlider;
    KioskSlideWidget *m_sunsetSlider;
    KioskLightSwitch *m_sunriseSwitch;
    KioskLightSwitch *m_sunsetSwitch;
};

class KioskDurationSchedule : public QWidget
{
    Q_OBJECT
public:
    KioskDurationSchedule(QWidget *parent=0);

private:
    KioskSlideWidget *m_durationSlider;
};

class KioskLightScheduleEditor : public KioskWidget
{
    Q_OBJECT
public:
    KioskLightScheduleEditor(QWidget *parent=0);

    void setupUI(KioskSliderButton*);

private slots:
    void slot_adjuster_changed(SliderNumber, int);
    void slot_slider_changed(int value=-1);
    void slot_slider_moved(int);

private:
    void saveDimmerRanges(void);

    QTabWidget *tabs;
    QWidget *m_slider;

    KioskSlideWidget  *m_adjuster;
    KioskSliderButton *m_button;
    KioskLightStatus  *m_status;

    KioskSettings *m_settings;

    KioskLightScheduleTab *m_sunSchedule;
    KioskLightScheduleTab *m_manualSchedule;
    KioskLightScheduleTab *m_durationSchedule;

signals:
    void sliderChanged(const QString, const LightType);
};

#endif
