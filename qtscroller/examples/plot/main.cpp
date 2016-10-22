
#include <QApplication>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSplitter>
#include <QStackedWidget>
#include <QSignalMapper>
#include <QMainWindow>
#include <QMenuBar>
#include <QActionGroup>
#include <QWebView>
#include <QTimer>
#include <QtScroller>

#ifdef Q_WS_MAEMO_5
// #  include <QAbstractKineticScroller>
#endif

#include <QGesture>

#include "settingswidget.h"
#include "plotwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(bool smallscreen, bool touch)
        : QMainWindow(), m_touch(touch)
    {
        m_list = new QListWidget();
        m_list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_list_scroller = installKineticScroller(m_list);

        for (int i = 0; i < 1000; ++i)
            new QListWidgetItem(QString("This is a test text %1 %2").arg(i).arg(QString("--------").left(i % 8)), m_list);

        m_web = new QWebView();
        m_web_scroller = installKineticScroller(m_web);

        QTimer::singleShot(1000, this, SLOT(loadUrl()));

        m_settings = new SettingsWidget(smallscreen);
        installKineticScroller(m_settings);
        m_plot = new PlotWidget(smallscreen);

        QStackedWidget *stack = new QStackedWidget();
        stack->addWidget(m_list);
        stack->addWidget(m_web);

        QActionGroup *pages = new QActionGroup(this);
        pages->setExclusive(true);
        QSignalMapper *mapper = new QSignalMapper(this);
        connect(mapper, SIGNAL(mapped(int)), stack, SLOT(setCurrentIndex(int)));

        createAction("List", pages, mapper, 0, true);
        createAction("Web",  pages, mapper, 1);

        if (smallscreen) {
            stack->addWidget(m_settings);
            stack->addWidget(m_plot);

            createAction("Settings", pages, mapper, 2);
            createAction("Plot",     pages, mapper, 3);

            setCentralWidget(stack);
        } else {
            QSplitter *split = new QSplitter();
            m_settings->setMinimumWidth(m_settings->sizeHint().width());
            split->addWidget(stack);
            split->addWidget(m_settings);
            split->addWidget(m_plot);
            setCentralWidget(split);
        }
        menuBar()->addMenu(QLatin1String("Pages"))->addActions(pages->actions());
        connect(stack, SIGNAL(currentChanged(int)), this, SLOT(pageChanged(int)));
        pageChanged(0);
    }

private slots:
    void pageChanged(int page)
    {
        if (page < 0 || page > 1)
            return;
        switch (page) {
        case 0:
            m_settings->setScroller(m_list->viewport());
            m_plot->setScroller(m_list->viewport());
            break;
        case 1:
            m_settings->setScroller(m_web);
            m_plot->setScroller(m_web);
            break;
        default:
            break;
        }
    }

    void loadUrl()
    {
        m_web->load(QUrl("http://www.google.com"));
    }

private:
    QAction *createAction(const char *text, QActionGroup *group, QSignalMapper *mapper, int mapping, bool checked = false)
    {
        QAction *a = new QAction(QLatin1String(text), group);
        a->setCheckable(true);
        a->setChecked(checked);
#if defined(Q_WS_MAC)
        a->setMenuRole(QAction::NoRole);
#endif
        mapper->setMapping(a, mapping);
        connect(a, SIGNAL(toggled(bool)), mapper, SLOT(map()));
        return a;
    }

    QtScroller *installKineticScroller(QWidget *w)
    {
        if (QAbstractScrollArea *area = qobject_cast<QAbstractScrollArea *>(w)) {
            QtScroller::grabGesture(area->viewport(), m_touch ? QtScroller::TouchGesture : QtScroller::LeftMouseButtonGesture);
            return QtScroller::scroller(area->viewport());
        } else if (QWebView *web = qobject_cast<QWebView *>(w)) {
            QtScroller::grabGesture(web, m_touch ? QtScroller::TouchGesture : QtScroller::LeftMouseButtonGesture);
        }
        return QtScroller::scroller(w);
    }

private:
    QListWidget *m_list;
    QWebView *m_web;
    QtScroller *m_list_scroller, *m_web_scroller;
    SettingsWidget *m_settings;
    PlotWidget *m_plot;
    bool m_touch;
};

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

#if defined(Q_WS_MAEMO_5) || defined(Q_WS_S60) || defined(Q_WS_WINCE)
    bool smallscreen = true;
#else
    bool smallscreen = false;
#endif
    bool touch = false;

    if (a.arguments().contains(QLatin1String("--small")))
        smallscreen = true;
    if (a.arguments().contains(QLatin1String("--touch")))
        touch = true;

    MainWindow *mw = new MainWindow(smallscreen, touch);
    if (smallscreen)
        mw->showMaximized();
    else
        mw->show();
#if defined(Q_WS_MAC)
    mw->raise();
#endif

    return a.exec();
}

#include "main.moc"
