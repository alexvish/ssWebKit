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


#ifndef WEBKITWIDGET_H
#define WEBKITWIDGET_H

#include <QtGui>
#include <QtWebKit>
#include <QtNetwork>
#include <QGLWidget>

class EventFilter;

class WebkitWidget:public QGraphicsView {
public:
    WebkitWidget();
    ~WebkitWidget();
    void closeOnMouseAndKeyboardEvents();
private:
    friend class EventFilter;
    void initUI();
    QGridLayout* layout;
    QWebView* view;
    QGLWidget* glWidget;
    EventFilter* eventFilter;
};

class EventFilter: public QObject {
public:
    EventFilter(WebkitWidget* w);
    ~EventFilter();
    bool eventFilter(QObject *obj, QEvent *event);
private:
    QPoint p;
};
#endif // WEBKITWIDGET_H
