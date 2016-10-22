/* */

#include "KioskWebBrowser.h"
#include <QWebPluginFactory>

#define GOOGLE_SEARCH "http://www.google.com/search?q="

QWebPageEx::QWebPageEx(QWidget *parent) : QWebPage(parent)
{
    QWebPage::settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    QWebPage::settings()->setAttribute(QWebSettings::JavascriptEnabled,true);
    QWebPage::settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, false);

    // from qt demo browser
    QString directory = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    if (directory.isEmpty())
        directory = QDir::homePath() + QLatin1String("/.") + QCoreApplication::applicationName();

    QWebPage::settings()->setIconDatabasePath(directory);
    QWebSettings::setOfflineStoragePath(directory);

    QNetworkAccessManagerEx *manager = new QNetworkAccessManagerEx();
    QWebPage::setNetworkAccessManager(manager);
}

#if 0
bool QWebPageEx::isTextField() const
{
   const WebCore::HTMLInputElement *input = selectedInput();

   if (input)
      return input->isTextField();

   return false;
}

bool QWebPageEx::isPasswordField() const
{
   const WebCore::HTMLInputElement *input = selectedInput();

   if (input)
      return input->isPasswordField();

   return false;
}

QString QWebPageEx::text() const
{
   const WebCore::HTMLInputElement *input = selectedInput();

   if (input)
      return input->value();

   return QString();


void QWebPageEx::setText(const QString& text)
{
   WebCore::HTMLInputElement *input = selectedInput();

   if (input) {
       input->setValue(text);
       input->dispatchFormControlChangeEvent();
   }
}

QString QWebPageEx::getAttribute(const QString& attrName)
{
   WebCore::Node *node = selectedNode();

   if (node && node->hasAttributes()) {
      WebCore::NamedNodeMap *attrs = node->attributes();

      if (attrs) {
         WTF::PassRefPtr<WebCore::Node> nd = attrs->getNamedItem(attrName);

         if (nd.get())
            return nd->nodeValue();
      }
   }

   return QString();
}

WebCore::Node* QWebPageEx::selectedNode() const
{
   WebCore::Frame *frame = handle()->page->focusController()->focusedOrMainFrame();
   if (frame) {
      WebCore::Document *document = frame->document();

      if (document) {
         WebCore::Node *node = document->focusedNode();

         if (node)
            return node;
      }
   }

   return NULL;
}

WebCore::HTMLInputElement* QWebPageEx::selectedInput() const
{
   WebCore::Node *node = selectedNode();

   if (node && node->hasTagName(WebCore::HTMLNames::inputTag))
      return static_cast<WebCore::HTMLInputElement*>(node);

   return NULL;
}
#endif

QNetworkAccessManagerEx::QNetworkAccessManagerEx(QObject *parent)
    : QNetworkAccessManager(parent)
{
    QNetworkAccessManager::setCookieJar(new CookieJar());
}

QNetworkReply *QNetworkAccessManagerEx::createRequest(Operation op,
  const QNetworkRequest &request, QIODevice *data)
{
    QNetworkRequest req = request;
    req.setRawHeader("Accept-Encoding", "identity");
    return QNetworkAccessManager::createRequest(op, req, data);
}

KioskWebView::KioskWebView(QWidget *parent) : QWebView(parent)
{
    QWebPageEx *wpage = new QWebPageEx();
    KioskWebView::setPage(wpage);

    QtScroller::grabGesture(this, QtScroller::LeftMouseButtonGesture);
}

KioskWebView *KioskWebView::createWindow(QWebPage::WebWindowType type)
{
    if (type == QWebPage::WebBrowserWindow) {
        KioskWebView *web = new KioskWebView();
        emit addTab(web);
        return web;
    } else
        return NULL;
}

KioskWebTab::KioskWebTab(QWidget *parent, KioskWebView *view)
{
    is_loading = false;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    if (view)
        web = view;
    else
        web = new KioskWebView();

    urlbar = new KioskLineEdit();
    searchbar = new KioskLineEdit();
    searchbar->setPlaceholderText("Google");
    searchbar->setIcon(QIcon(":/res/google.ico"));

    edits_layout = new QHBoxLayout();
    edits_layout->setMargin(0);
    edits_layout->setSpacing(0);

    edits_layout->addWidget(urlbar, 100);
    edits_layout->addSpacerItem(new QSpacerItem(8, 0));
    edits_layout->addWidget(searchbar, 25);

    layout->addLayout(edits_layout);

    layout->addWidget(web, 1);

    connect(urlbar, SIGNAL(returnPressed()), this, SLOT(slot_url_bar()));
    connect(urlbar, SIGNAL(activated()), this, SLOT(slot_urlbar_activated()));
    connect(urlbar, SIGNAL(deactivated()), this, SLOT(slot_urlbar_deactivated()));
    connect(searchbar, SIGNAL(returnPressed()), this, SLOT(slot_search_bar()));
    connect(searchbar, SIGNAL(activated()), this, SLOT(slot_search_activated()));
    connect(searchbar, SIGNAL(deactivated()), this, SLOT(slot_search_deactivated()));
    connect(web, SIGNAL(urlChanged(const QUrl&)), this, SLOT(slot_url_changed(const QUrl&)));
    connect(web, SIGNAL(loadProgress(int)), this, SLOT(slot_load_progress(int)));
    connect(web, SIGNAL(loadFinished(bool)), this, SLOT(slot_icon_changed()));
    connect(web, SIGNAL(titleChanged(const QString&)), this, SLOT(slot_title_changed(const QString&)));
    connect(web, SIGNAL(iconChanged()), this, SLOT(slot_icon_changed()));
    connect(web, SIGNAL(addTab(KioskWebView*)), this, SIGNAL(addTab(KioskWebView*)));
}

bool KioskWebTab::canGoBack(void)
{
    return web->page()->history()->canGoBack();
}

bool KioskWebTab::canGoForward(void)
{
    return web->page()->history()->canGoForward();
}

void KioskWebTab::slot_urlbar_activated(void)
{
    edits_layout->setStretch(2, 25);
    edits_layout->setStretch(0, 100);
}

void KioskWebTab::slot_urlbar_deactivated(void)
{
    hideKeyboard();
}

void KioskWebTab::slot_search_activated(void)
{
    edits_layout->setStretch(0, 25);
    edits_layout->setStretch(2, 100);
}

void KioskWebTab::slot_search_deactivated(void)
{
    edits_layout->setStretch(2, 25);
    edits_layout->setStretch(0, 100);
    hideKeyboard();
}

void KioskWebTab::slot_load_progress(int percentage)
{
    urlbar->setProgress(percentage);
}

void KioskWebTab::slot_url_bar(void)
{
    QString url = urlbar->text();
    if (!url.contains(":/"))
        url.prepend("http://");

    urlbar->setText(url);
    web->load(QUrl(url));
}

void KioskWebTab::slot_search_bar(void)
{
    QString search = searchbar->text();
    search.prepend(GOOGLE_SEARCH);
    web->load(QUrl(search));
    slot_search_deactivated();
}

void KioskWebTab::slot_url_changed(const QUrl &url)
{
    urlbar->setText(url.toString());

    emit enableBackButton(this, web->page()->history()->canGoBack());
    emit enableForwardButton(this, web->page()->history()->canGoForward());
}

void KioskWebTab::slot_loading(void)
{
    is_loading = true;
    emit isLoading(this, is_loading);
}

void KioskWebTab::slot_loaded(bool ok)
{
    is_loading = false;
    emit isLoading(this, is_loading);
}

void KioskWebTab::slot_title_changed(const QString &title)
{
    QString tab_name = title;
    if (tab_name.length() > 20) {
        tab_name.truncate(17);
        tab_name.append("...");
    }

    emit changeName(this, tab_name);
}

void KioskWebTab::slot_icon_changed(void)
{
    QIcon icon = web->icon();
    urlbar->setIcon(icon);
    emit changeIcon(this, icon);
}

KioskWebBrowser::KioskWebBrowser(QWidget *tools)
    : KioskWidget()
{
    KioskWidget::setObjectName("Web Browser");

    KioskWidget::setUsesKeyboard(true);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(0);

    back_button = new KioskToolButton(this, ":/res/nav_back.png", "Back", this, SLOT(slot_back_button()));
    forward_button = new KioskToolButton(this, ":/res/nav_forward.png", "Forward", this, SLOT(slot_forward_button()));
    reload_button = new KioskToolButton(this, ":/res/nav_reload.png", "Reload", this, SLOT(slot_reload_button()));
    textsize_minus = new KioskButton();
    textsize_plus = new KioskButton();

    textsize_minus->setText("ABC");
    QFont mfont = textsize_minus->font();
    mfont.setPointSize(12);
    textsize_minus->setFont(mfont);

    textsize_plus->setText("ABC");
    QFont pfont = textsize_plus->font();
    pfont.setPointSize(16);
    textsize_plus->setFont(pfont);

    textsize_minus->setEnabled(false);

    if (tools) {
        QHBoxLayout *tools_layout = new QHBoxLayout(tools);
        tools_layout->setMargin(0);

        tools_layout->addWidget(back_button);
        tools_layout->addWidget(forward_button);
        tools_layout->addWidget(reload_button);
        tools_layout->addSpacerItem(new QSpacerItem(10, 1));
        tools_layout->addWidget(textsize_minus);
        tools_layout->addWidget(textsize_plus);
    }

    back_button->setEnabled(false);
    forward_button->setEnabled(false);

    tabs = new QTabWidget();
    tabs->setTabPosition(QTabWidget::North);
    tabs->setTabsClosable(true);

    KioskButton *new_button = new KioskButton();
    new_button->setMargin(0);
    new_button->setOpacity(0);
    new_button->setIcon(QIcon(":/res/tab_add.png"));
    new_button->setIconSize(QSize(34, 34));

    tabs->setCornerWidget(new_button);

    vbox->addWidget(tabs);

    (void)addTab();

    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(slot_tab_changed(int)));
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(slot_tab_close(int)));
    connect(textsize_minus, SIGNAL(clicked()), this, SLOT(slot_textsize_minus()));
    connect(textsize_plus, SIGNAL(clicked()), this, SLOT(slot_textsize_plus()));
    connect(new_button, SIGNAL(clicked()), this, SLOT(addTab()));
}

KioskWebTab *KioskWebBrowser::addTab(KioskWebView *view)
{
    KioskWebTab *page = new KioskWebTab(0, view);

    tabs->setCurrentIndex(tabs->addTab(page, "Loading..."));
    tabs->setTabIcon(tabs->currentIndex(), QIcon(":/res/tab_icon.png"));

    loadUrl(QUrl("qrc:/res/welcome.html"));

    connect(page, SIGNAL(addTab(KioskWebView*)), this, SLOT(addTab(KioskWebView*)));
    connect(page, SIGNAL(changeName(KioskWebTab *, QString)), this, SLOT(slot_change_tab_name(KioskWebTab *, QString)));
    connect(page, SIGNAL(changeIcon(KioskWebTab *, QIcon)), this, SLOT(slot_change_tab_icon(KioskWebTab *, QIcon)));
    connect(page, SIGNAL(enableBackButton(KioskWebTab *, bool)), this, SLOT(slot_enable_back(KioskWebTab *, bool)));
    connect(page, SIGNAL(enableForwardButton(KioskWebTab *, bool)), this, SLOT(slot_enable_forward(KioskWebTab *, bool)));
    connect(page, SIGNAL(isLoading(KioskWebTab *, bool)), this, SLOT(slot_loading(KioskWebTab *, bool)));
    connect(page, SIGNAL(showKeyboard()), this, SIGNAL(requestShowKeyboard()));
    connect(page, SIGNAL(hideKeyboard()), this, SIGNAL(requestHideKeyboard()));

    return page;
}

void KioskWebBrowser::loadUrl(QUrl url)
{
    QWidget *page = tabs->currentWidget();

    if (page) {
        ((KioskWebTab*)page)->webView()->load(url);
        ((KioskWebTab*)page)->webView()->history()->clear();
    }
#warning: fix me
#warning: show error on failed load
}

void KioskWebBrowser::reset(void)
{
    tabs->setCurrentIndex(0);
    loadUrl(QUrl("qrc:/res/welcome.html"));

    QWidget *page = tabs->currentWidget();
    ((KioskWebTab*)page)->searchBar()->clear();

    int count = tabs->count();

    for (int i = (count - 1); i > 0; i--) {
        QWidget *page = tabs->widget(i);
        tabs->removeTab(i);
        page->deleteLater();
    }
}

void KioskWebBrowser::slot_tab_close(int index)
{
    QWidget *page = tabs->widget(index);

    if (!index) {
        tabs->addTab(page, "Loading...");
        loadUrl(QUrl("qrc:/res/welcome.html"));
        tabs->setTabIcon(tabs->currentIndex(), QIcon(":/res/tab_icon.png"));
        ((KioskWebTab*)page)->webView()->history()->clear();
    } else
        page->deleteLater();
}

void KioskWebBrowser::slot_change_tab_name(KioskWebTab *page, QString name)
{
    tabs->setTabText(tabs->indexOf(page), name);
}

void KioskWebBrowser::slot_change_tab_icon(KioskWebTab *page, QIcon icon)
{
    tabs->setTabIcon(tabs->indexOf(page), icon);
}

void KioskWebBrowser::slot_tab_changed(int index)
{
    QWidget *page = tabs->widget(index);

    if (page) {
        slot_loading((KioskWebTab*)page, ((KioskWebTab*)page)->isLoading());
        slot_enable_back((KioskWebTab*)page, ((KioskWebTab*)page)->canGoBack());
        slot_enable_forward((KioskWebTab*)page, ((KioskWebTab*)page)->canGoForward());
    }
}

void KioskWebBrowser::slot_enable_back(KioskWebTab *page, bool enable)
{
    if (page == tabs->currentWidget())
        back_button->setEnabled(enable);
}

void KioskWebBrowser::slot_enable_forward(KioskWebTab *page, bool enable)
{
    if (page == tabs->currentWidget())
        forward_button->setEnabled(enable);
}

void KioskWebBrowser::slot_loading(KioskWebTab *page, bool loading)
{
    if (page == tabs->currentWidget()) {
        if (loading) {
            reload_button->setIcon(QIcon(":/res/nav_cancel.png"));
            reload_button->setText("Stop");
        } else {
            reload_button->setIcon(QIcon(":/res/nav_reload.png"));
            reload_button->setText("Reload");
        }
    }
}

void KioskWebBrowser::slot_back_button(void)
{
    QWidget *page = tabs->currentWidget();
    if (page)
        ((KioskWebTab*)page)->webView()->back();
}

void KioskWebBrowser::slot_forward_button(void)
{
    QWidget *page = tabs->currentWidget();
    if (page)
        ((KioskWebTab*)page)->webView()->forward();
}

void KioskWebBrowser::slot_reload_button(void)
{
    QWidget *page = tabs->currentWidget();

    if (page) {
        if (((KioskWebTab*)page)->isLoading())
            ((KioskWebTab*)page)->webView()->stop();
        else
            ((KioskWebTab*)page)->webView()->reload();
    }
}

void KioskWebBrowser::slot_textsize_minus(void)
{
    QWidget *page = tabs->currentWidget();

    if (page) {
        float text_size = ((KioskWebTab*)page)->webView()->textSizeMultiplier() - .1;
        textsize_minus->setEnabled((text_size != 1.0));
        textsize_plus->setEnabled((int(text_size) != 2.0));

        ((KioskWebTab*)page)->webView()->setTextSizeMultiplier(text_size);
    }
}

void KioskWebBrowser::slot_textsize_plus(void)
{
    QWidget *page = tabs->currentWidget();

    if (page) {
        float text_size = ((KioskWebTab*)page)->webView()->textSizeMultiplier() + .1;
        textsize_minus->setEnabled((text_size != 1.0));
        textsize_plus->setEnabled((int(text_size) != 2.0));

        ((KioskWebTab*)page)->webView()->setTextSizeMultiplier(text_size);
    }
}



