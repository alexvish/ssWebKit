#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QList>
#include <QPointer>
#include "webkitwidget.h"

class ScreenSaver {
public:
    ScreenSaver(QApplication* app);
    ScreenSaver(QApplication* app, WId parentWid);
    ~ScreenSaver();
    void closeOnMouseAndKeyboardEvents();
private:
    QList<QPointer<WebkitWidget> > widgets;
};

#endif // SCREENSAVER_H
