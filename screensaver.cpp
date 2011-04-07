#include "screensaver.h"
#include <qalgorithms.h>
#include <windows.h>

ScreenSaver::ScreenSaver(QApplication* app){
    QDesktopWidget desktop;
    for (int i = 0, n = desktop.numScreens(); i < n; i++) {
        WebkitWidget* w = new WebkitWidget();
        widgets.prepend(w);

        w->setAttribute(Qt::WA_DeleteOnClose);
        QObject::connect(w,SIGNAL(destroyed()), app, SLOT(quit()));
        w->move(desktop.screenGeometry(i).topLeft());
        w->showFullScreen();
    }
    app -> setOverrideCursor(QCursor(Qt::BlankCursor));
}

ScreenSaver::ScreenSaver(QApplication* app, WId parent_Wid){
    WebkitWidget* w = new WebkitWidget();
    widgets.prepend(w);

    w->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(w,SIGNAL(destroyed()), app, SLOT(quit()));

    w->setWindowFlags(Qt::FramelessWindowHint|Qt::SubWindow);
    ::SetParent(w->winId(),parent_Wid);
    RECT parentRect;
    GetClientRect(parent_Wid,&parentRect);
    w->move(0,0);
    w->resize(parentRect.right,parentRect.bottom);
    w->show();

}

ScreenSaver::~ScreenSaver() {
    qDeleteAll(widgets);
}

void ScreenSaver::closeOnMouseAndKeyboardEvents() {
    for(int i = 0; i < widgets.size(); i++) {
        if (widgets[i]) {
            widgets[i] -> closeOnMouseAndKeyboardEvents();
        }
    }
}
