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
