/* */

#ifndef _KIOSK_WEBBROWSER_H_
#define _KIOSK_WEBBROWSER_H_

#include "qwebkitglobal.h"
#include <QWebView>
#include <QWebHistory>
#include <QWebElement>
#include <QWebFrame>
#include <QtNetwork>
#include <QtGui>
#include <QX11EmbedContainer>
#include <QtScroller>

#include "KioskGUI.h"
#include "KioskLineEdit.h"

#include "cookiejar.h"

#if 0
#include "HTMLInputElement.h"

namespace WebCore
{
    class Node;
    class HTMLInputElement;
}

namespace WTF {
    template <typename T> class PassRefPtr;
}
#endif

class KioskFlickableWeb : public QWidget
{
    Q_OBJECT
public:
    KioskFlickableWeb(QWidget *parent=0);
};

class QWebPageEx : public QWebPage
{
    Q_OBJECT
public:
    QWebPageEx(QWidget *parent=0);
#if 0
    bool isTextField() const;
    bool isPasswordField() const;

    QString text() const;
    void setText(const QString& text);
    QString getAttribute(const QString& attrName);

private:
    WebCore::Node* selectedNode() const;
    WebCore::HTMLInputElement* selectedInput() const;
#endif
};

class QNetworkAccessManagerEx : public QNetworkAccessManager
{
    Q_OBJECT
public:
    QNetworkAccessManagerEx(QObject *parent=0);
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &request,
        QIODevice *data);
};

class KioskWebView : public QWebView
{
    Q_OBJECT
public:
    KioskWebView(QWidget *parent=0);

public slots:
    virtual KioskWebView *createWindow(QWebPage::WebWindowType);

signals:
    void addTab(KioskWebView*);
};

class KioskWebTab : public QWidget
{
    Q_OBJECT
public:
    KioskWebTab(QWidget *parent=0, KioskWebView *view=0);

    KioskWebView *webView(void) {return web;}
    KioskLineEdit *urlBar(void) {return urlbar;}
    KioskLineEdit *searchBar(void) {return searchbar;}

    bool isLoading(void) {return is_loading;}
    bool canGoForward(void);
    bool canGoBack(void);

private slots:
    void slot_loading(void);
    void slot_loaded(bool ok);
    void slot_url_changed(const QUrl&);
    void slot_url_bar(void);
    void slot_search_bar(void);
    void slot_load_progress(int);
    void slot_title_changed(const QString&);
    void slot_icon_changed(void);
    void slot_urlbar_activated(void);
    void slot_urlbar_deactivated(void);
    void slot_search_activated(void);
    void slot_search_deactivated(void);

private:
    KioskWebView *web;
    KioskLineEdit *urlbar;
    KioskLineEdit *searchbar;

    QHBoxLayout *edits_layout;

    bool is_loading;

signals:
    void addTab(KioskWebView *view);
    void enableBackButton(KioskWebTab*, bool);
    void enableForwardButton(KioskWebTab*, bool);
    void isLoading(KioskWebTab*, bool);
    void changeName(KioskWebTab*, QString);
    void changeIcon(KioskWebTab*, QIcon);
    void showKeyboard(void);
    void hideKeyboard(void);
};

class KioskWebBrowser : public KioskWidget
{
    Q_OBJECT
public:
    KioskWebBrowser(QWidget *tools=0);
    void loadUrl(QUrl);
    void reset(void);

public slots:
    KioskWebTab *addTab(KioskWebView *view=0);

private slots:
    void slot_tab_close(int);
    void slot_change_tab_name(KioskWebTab*, QString);
    void slot_change_tab_icon(KioskWebTab*, QIcon);
    void slot_tab_changed(int);
    void slot_loading(KioskWebTab*, bool);
    void slot_enable_back(KioskWebTab*, bool);
    void slot_enable_forward(KioskWebTab*, bool);
    void slot_back_button(void);
    void slot_forward_button(void);
    void slot_reload_button(void);
    void slot_textsize_minus(void);
    void slot_textsize_plus(void);

private:
    void hideKeyboard(void);
    void showKeyboard(void);

    QTabWidget *tabs;

    KioskToolButton *back_button;
    KioskToolButton *forward_button;
    KioskToolButton *reload_button;

    KioskButton *textsize_minus;
    KioskButton *textsize_plus;

signals:
    void requestShowKeyboard(void);
    void requestHideKeyboard(void);
};

#endif
