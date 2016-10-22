/* */

#include <QApplication>

#include "stdio.h"

#include "KioskWindow.h"
#include "KioskMainMenu.h"

#include "TestMeters.h"

#include <QtGui>

void print_help(void)
{
    printf("Usage:\n"
           "     -h : Show this help screen.\n"
           "     -s : XScreenSaver to use.\n"
           "     -t : Set background transparent. Can't be used with -s.\n"
           "     -c : Show mouse cursor.\n"
           "     -m : Show background window maximized instead of fullscreen.\n\n");
}

int main(int argc, char *argv[])
{
    bool show_maximized = false;
    bool show_cursor = false;
    bool transparent = false;
    QString background_app;

    QApplication *app = new QApplication(argc, argv);

    QStringListIterator iterator(app->arguments());
    while (iterator.hasNext()) {
        QString arg = iterator.next();

        if (!arg.compare("-h", Qt::CaseInsensitive)) {
            print_help();
            return 0;
        } else if (!arg.compare("-c", Qt::CaseInsensitive)) {
            show_cursor = true;
        } else if (!arg.compare("-m", Qt::CaseInsensitive)) {
            show_maximized = true;
        } else if (!arg.compare("-s", Qt::CaseInsensitive)) {
            background_app = iterator.next();
        } else if (!arg.compare("-t", Qt::CaseInsensitive)) {
            transparent = true;
        }
    }

    if (!background_app.isEmpty() && transparent) {
        printf("Options -s and -t cannot be used together.\n\n");
        print_help();
        return 1;
    }

    app->setGraphicsSystem("raster");

    if (!show_cursor)
        app->setOverrideCursor(QCursor(Qt::BlankCursor));

    KioskWindow *main = new KioskWindow(0, show_cursor);
    //main->setWindowFlags(Qt::FramelessWindowHint);

    app->setActiveWindow(main);

    //main->setWindowFlags(Qt::WindowStaysOnTopHint);
    main->addWidget(new MainMenu());
    //main->addWidget(new TestMeters());
    main->showFullScreen();

    if (!transparent) {
        KioskBackground *background = new KioskBackground();
        background->setAttribute(Qt::WA_ShowWithoutActivating);
        background->setFocusPolicy(Qt::NoFocus);

        if (show_maximized) {
            background->setWindowFlags(Qt::FramelessWindowHint);
            background->showMaximized();
        } else
            background->showFullScreen();

        background->lower();
        background->setApp(background_app);
    }

    return app->exec();
}

