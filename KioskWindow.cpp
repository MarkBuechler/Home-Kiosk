/* */

#include "KioskWindow.h"

#define IDLE_TIMEOUT 300000

KioskWindow::KioskWindow(QWidget *parent, bool cursor)
    : QWidget(),
    split(0),
    m_scroller(0),
    m_db(new KioskDBLink()),
    m_first(true),
    m_notesVisible(true)
{
    is_pressed = false;
    is_idled = false;

    QCoreApplication::setOrganizationName(QLatin1String("Mentasm"));
    QCoreApplication::setApplicationName(QLatin1String("Kiosk"));
    QCoreApplication::setApplicationVersion(QLatin1String("0.1"));

    m_db->connect();

    m_settings = new QSettings();

    //QWidget::setMouseTracking(true);

    qApp->setStyle(new KioskStyle());

    QWidget::setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *top = new QVBoxLayout(this);
    top->setMargin(20);
    top->setSpacing(0);

    toolbar_widget = new KioskToolbar();
    toolbar_widget->insertSpacerItem(0, new QSpacerItem(10, 1));
    toolbar_widget->previousButton()->hide();

    browser_tools = new QWidget();

    toolbar_widget->insertWidget(0, browser_tools);

    top->addWidget(toolbar_widget);

    keyboard_button = new KioskToolButton(this, ":/res/keyboard.png", "Keyboard", this, SLOT(slot_keyboard()));
    keyboard_button->setCheckable(true);

    toolbar_widget->insertWidget(0, keyboard_button);

    QHBoxLayout *main = new QHBoxLayout();
    main->setMargin(0);

    QVBoxLayout *menu_layout = new QVBoxLayout();
    menu_layout->setMargin(0);

    main->addLayout(menu_layout, 2);

    split = new QSplitter();
    split->setOrientation(Qt::Vertical);

    QWidget *split_widget = new QWidget();
    QVBoxLayout *split_layout = new QVBoxLayout(split_widget);

    stack = new QStackedWidget();
    stack->setStyleSheet("background: transparent"); // necessary
    keyboard_widget = new KioskVirtualKeyboard(this);

    browser_widget = new KioskWebBrowser(browser_tools);

    m_scene = new QGraphicsScene();
    m_stackProxy = m_scene->addWidget(stack);

    m_scroller = new KioskGraphicsView();
    m_scroller->viewport()->setStyleSheet("background: transparent");
    m_scroller->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scroller->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //m_scroller->setOvershoot(true);
    m_scroller->setScene(m_scene);

    QGraphicsWidget *topWidget = new QGraphicsWidget(0, Qt::Window);
    QGraphicsAnchorLayout *notesLayout = new QGraphicsAnchorLayout();
    topWidget->setLayout(notesLayout);

    m_qmlViewer = new KioskDeclarativeView();

    split_layout->addWidget(m_scroller);
    split_layout->addWidget(m_qmlViewer);
    split_layout->addWidget(browser_widget);

    split->addWidget(split_widget);
    split->addWidget(keyboard_widget);

    menu_layout->addWidget(split, 1);

    m_qmlViewer->hide();
    browser_widget->hide();
    browser_tools->hide();

    hideKeyboard();

    top->addLayout(main, 7);

    if (!cursor)
        setCursor(QCursor(Qt::BlankCursor));

    KioskEventFilter *filter = new KioskEventFilter();
    qApp->installEventFilter(filter);

    connect(filter, SIGNAL(resetIdle()), this, SLOT(slot_reset_idle()));
    connect(filter, SIGNAL(showKeyboard()), this, SLOT(slot_keyboard_show_request()));

    idleTimer = new QTimer();
    connect(idleTimer, SIGNAL(timeout()), this, SLOT(slot_idle()));
    idleTimer->start(IDLE_TIMEOUT);

    m_notepadButton = new KioskGraphicsButton();
    m_notepadButton->setSvg(QLatin1String(":/res/notepad.svg"));
    m_scene->addItem(m_notepadButton);

    m_keyboardButton = new KioskGraphicsButton();
    m_keyboardButton->setSvg(QLatin1String(":/res/keyboard.svg"));
    m_scene->addItem(m_keyboardButton);

    notesLayout->addCornerAnchors(m_notepadButton, Qt::TopLeftCorner, notesLayout, Qt::TopLeftCorner);

    notesLayout->addAnchor(m_keyboardButton, Qt::AnchorLeft, m_notepadButton, Qt::AnchorRight);
    notesLayout->addAnchor(m_keyboardButton, Qt::AnchorTop, m_notepadButton, Qt::AnchorTop);

    //notesLayout->addCornerAnchors(m_keyboardButton, Qt::TopRightCorner, notesLayout, Qt::TopRightCorner);

    slot_new_sticky();
    slot_toggle_notes();

    connect(toolbar_widget, SIGNAL(previousWindow()), this, SLOT(slot_previous()));
    connect(keyboard_widget, SIGNAL(requestHide()), this, SLOT(slot_keyboard_hide_request()));
    connect(browser_widget, SIGNAL(requestShowKeyboard()), this, SLOT(slot_keyboard_show_request()));
    connect(browser_widget, SIGNAL(requestHideKeyboard()), this, SLOT(slot_keyboard_hide_request()));

    connect(m_notepadButton, SIGNAL(clicked()), this, SLOT(slot_toggle_notes()));
    connect(m_keyboardButton, SIGNAL(clicked()), keyboard_button, SLOT(toggle()));
}

void KioskWindow::addWidget(QWidget *widget)
{
    QWidget::showMinimized();

    QWidget *top = stack->currentWidget();

    if (top) {
        toolbar_widget->previousButton()->setText(top->objectName());
        top->hide();
    }

    if (m_notesVisible)
        slot_toggle_notes();

    stack->addWidget(widget);
    stack->setCurrentIndex(stack->count() - 1);

    if (stack->count() > 1)
        toolbar_widget->previousButton()->show();

    if (((KioskWidget*)widget)->usesKeyboard())
        keyboard_button->show();
    else
        keyboard_button->hide();

    if (!((KioskWidget*)widget)->usesNotepad())
        m_notepadButton->hideButton();
    else
        m_notepadButton->showButton();

    if (((KioskWidget*)widget)->isStatusScreen()) {
        is_idled = true;
        if (m_notesVisible) slot_toggle_notes();
        m_notepadButton->hideButton();
    }

    ((KioskWidget*)widget)->setDBLink(m_db);

    connect(widget, SIGNAL(changeKiosk(QWidget*)), this, SLOT(addWidget(QWidget*)));
    connect(widget, SIGNAL(changeBrowser(QUrl)), this, SLOT(showBrowser(QUrl)));
    connect(widget, SIGNAL(changeQml(KioskQmlItem*)), this, SLOT(showQml(KioskQmlItem*)));
    connect(widget, SIGNAL(requestShowKeyboard()), this, SLOT(slot_keyboard_show_request()));
    connect(widget, SIGNAL(requestHideKeyboard()), this, SLOT(slot_keyboard_hide_request()));

    QWidget::showFullScreen();

    stack->setFixedSize(m_scroller->maximumViewportSize());
}

void KioskWindow::hideToolbar(void)
{
    toolbar_widget->hide();
}

void KioskWindow::showToolbar(void)
{
    toolbar_widget->show();
}

void KioskWindow::showKeyboard(void)
{
    keyboard_widget->show();

    QList<int> sizes;
    int height = split->height();
    sizes << (height / 10 * 7) << (height / 10 * 3);
    split->setSizes(sizes);
}

void KioskWindow::hideKeyboard(void)
{
    keyboard_widget->hide();
    keyboard_widget->reset();
}

void KioskWindow::showQml(KioskQmlItem *item)
{
    QWidget::showMinimized();

    QWidget *top = stack->currentWidget();

    if (top) {
        toolbar_widget->previousButton()->setText(top->objectName());
        toolbar_widget->previousButton()->show();
        m_scroller->hide();
        browser_widget->hide();
        browser_tools->hide();
        (void)m_qmlViewer->setQmlItem(item);
        m_qmlViewer->show();
    }

    if (item->usesKeys())
        keyboard_button->show();
    else
        keyboard_button->hide();

    QWidget::showFullScreen();
}

void KioskWindow::hideQml(void)
{
    QWidget::showMinimized();

    m_qmlViewer->hide();
    m_qmlViewer->reset();

    QWidget *top = stack->currentWidget();

    if (top) {
        toolbar_widget->previousButton()->setText(top->objectName());

        if (((KioskWidget*)top)->usesKeyboard())
            keyboard_button->show();
        else
            keyboard_button->hide();
    }

    m_scroller->show();

    setWindowOpacity(0.85);

    QWidget::showFullScreen();
}

void KioskWindow::showBrowser(QUrl url)
{
    QWidget::showMinimized();

    setWindowOpacity(1.0);

    QWidget *top = stack->currentWidget();

    if (top) {
        m_scroller->hide();
        toolbar_widget->previousButton()->setText(top->objectName());
        toolbar_widget->previousButton()->show();
        browser_widget->show();
        browser_tools->show();

        if (!url.isEmpty())
            browser_widget->loadUrl(url);
    }

    if (browser_widget->usesKeyboard())
        keyboard_button->show();
    else
        keyboard_button->hide();

    QWidget::showFullScreen();
}

void KioskWindow::hideBrowser(void)
{
    QWidget::showMinimized();

    browser_widget->hide();
    browser_tools->hide();
    browser_widget->reset();

    QWidget *top = stack->currentWidget();

    if (top) {
        toolbar_widget->previousButton()->setText(top->objectName());

        if (((KioskWidget*)top)->usesKeyboard())
            keyboard_button->show();
        else
            keyboard_button->hide();
    }

    m_scroller->show();

    setWindowOpacity(0.85);

    QWidget::showFullScreen();
}

void KioskWindow::slot_keyboard_hide_request()
{
    keyboard_button->setChecked(false);
}

void KioskWindow::slot_keyboard_show_request()
{
    keyboard_button->setChecked(true);
}

void KioskWindow::slot_keyboard(void)
{
    if(keyboard_button->isChecked())
        showKeyboard();
    else
        hideKeyboard();
}

void KioskWindow::slot_previous(void)
{
    QWidget::showMinimized();

    keyboard_button->setChecked(false);

    QWidget *top = stack->currentWidget();

    if (browser_widget->isVisible()) {
        hideBrowser();
        m_scroller->show();
    } else if (m_qmlViewer->isVisible()) {
        hideQml();
        m_scroller->show();
    } else if (top) {
        stack->removeWidget(top);
        top->deleteLater();
    }

    if (((KioskWidget*)top)->isStatusScreen())
        is_idled = false;

    top = stack->currentWidget();

    if (((KioskWidget*)top)->usesKeyboard())
        keyboard_button->show();
    else
        keyboard_button->hide();

    if (!((KioskWidget*)top)->isStatusScreen())
        m_notepadButton->showButton();

    if (m_notesVisible)
        top->setEnabled(false);

    QWidget *previous = stack->widget(stack->count() - 2);

    if (previous)
        toolbar_widget->previousButton()->setText(previous->objectName());
    else
        toolbar_widget->previousButton()->hide();

    QWidget::showFullScreen();

    stack->setFixedSize(m_scroller->maximumViewportSize());
}

void KioskWindow::slot_reset_idle(void)
{
    idleTimer->start(IDLE_TIMEOUT);
}

void KioskWindow::slot_idle(void)
{
    if (!is_idled) {
        hideKeyboard();
        hideBrowser();
        addWidget(new KioskStatusWidget());
    }
}

void KioskWindow::slot_new_sticky(void)
{
    KioskStickyNote *sticky = new KioskStickyNote();
    sticky->setEditing(false);
    m_scene->addItem(sticky);

    QSize viewableSize = m_scroller->maximumViewportSize();

    if (m_first) {
        sticky->initialize(QPointF());
        m_first = false;
        slot_new_sticky();
        sticky->setPos(400, 400);
    } else
        sticky->setPos(800, 20);

    connect(sticky, SIGNAL(requestShowKeyboard()), this, SLOT(slot_keyboard_show_request()));
    connect(sticky, SIGNAL(requestHideKeyboard()), this, SLOT(slot_keyboard_hide_request()));
    connect(sticky, SIGNAL(initialized()), this, SLOT(slot_new_sticky()));
}

void KioskWindow::slot_toggle_notes(void)
{
    QList<QGraphicsItem*> items = m_scene->items();

    for (int i = 0; i < items.size(); ++i) {
        QGraphicsItem *item = items.at(i);
        QString objectName = ((QGraphicsObject*)item)->objectName();

        if (!objectName.compare("StickyNote")) {
                if (m_notesVisible)
                    ((KioskStickyNote*)item)->hideSticky();
                else
                    ((KioskStickyNote*)item)->showSticky();
            }
    }

    if (stack->currentWidget())
        stack->currentWidget()->setEnabled(m_notesVisible);
#if 0 // much too slow
    if (!m_notesVisible) {
        QGraphicsBlurEffect *blurred = new QGraphicsBlurEffect();
        blurred->setBlurHints(QGraphicsBlurEffect::AnimationHint|QGraphicsBlurEffect::PerformanceHint);
        m_stackProxy->setGraphicsEffect(blurred);
    } else
        m_stackProxy->setGraphicsEffect(NULL);
#endif
    //m_scroller->setOvershoot(m_notesVisible);

    if (m_notesVisible)
        m_keyboardButton->hideButton();
    else
        m_keyboardButton->showButton();

    m_notesVisible = !m_notesVisible;
}

void KioskWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    stack->setFixedSize(m_scroller->maximumViewportSize());
}


