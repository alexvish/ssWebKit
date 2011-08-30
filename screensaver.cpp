/**
 *  ssWebKit - WebKit based Windows screensaver
 *  Copyright (C) 20011  Alexey Vishentsev
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#include "screensaver.h"
#include <qalgorithms.h>
#include <windows.h>


ScreenSaver::ScreenSaver(QApplication* app){
    for (int i = 0, n = QApplication::desktop()->numScreens(); i < n; i++) {
        WebkitWidget* w = new WebkitWidget();
        widgets.prepend(w);
        w->setAttribute(Qt::WA_DeleteOnClose);
        QObject::connect(w,SIGNAL(destroyed()), app, SLOT(quit()));
        QRect screenRes = QApplication::desktop()->screenGeometry(i);
        w->move(screenRes.x(),screenRes.y());
        w->resize(screenRes.width(),screenRes.height());
        w->showFullScreen();
    }
    app -> setOverrideCursor(QCursor(Qt::BlankCursor));
}


ScreenSaver::ScreenSaver(QApplication* app, WId parent_Wid){
    WebkitWidget* w = new WebkitWidget(parent_Wid);
    widgets.prepend(w);
    w->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(w,SIGNAL(destroyed()), app, SLOT(quit()));
    RECT parentRect;
    GetClientRect(parent_Wid,&parentRect);
    LONG width = parentRect.right - parentRect.left;
    LONG height = parentRect.bottom - parentRect.top;
    QRect screenRes = QApplication::desktop()->screenGeometry();
    w->scale(qreal(width)/screenRes.width(),qreal(height)/screenRes.height());
    w->resize(width,height);
    w->move(0,0);
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
