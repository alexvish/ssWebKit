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

#include "webkitwidget.h"


WebkitWidget::WebkitWidget()
    :QGraphicsView()
    ,eventFilter(NULL) {
    this->initUI();
}

WebkitWidget::~WebkitWidget() {
    if (eventFilter) {
        delete eventFilter;
    }
    destroy();
    delete view;
    delete layout;
}


void WebkitWidget::initUI() {
    QSettings settings("QT","ssWebKit");
    QString urlString = settings.value("url","qrc://res/web-content/html/slides.html").toString();
    QUrl url = QUrl(urlString);

    layout = new QGridLayout(this);
    layout->setContentsMargins(0,0,0,0);

    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled,true);

    view = new QWebView(this);
    view->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    layout->addWidget(view,0,0,1,1);
    view->load(url);

}

void WebkitWidget::closeOnMouseAndKeyboardEvents() {
    if (!eventFilter) {
        eventFilter = new EventFilter(this);
    }
}


EventFilter::EventFilter (WebkitWidget* w)
    :QObject(w)
    ,p(-1,-1)
{
    w->view->installEventFilter(this);
}

EventFilter::~EventFilter() {
}

bool EventFilter::eventFilter(QObject *obj, QEvent *event) {
    // quit if key is pressed or mouse is moved or pressed
    if (event->type() == QEvent::KeyPress
            || event->type() == QEvent::MouseButtonPress) {


        qobject_cast<QWidget*>(this->parent())->close();
    }

    if( event->type()==QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(p != QPoint(-1, -1) && (p-mouseEvent->pos()).manhattanLength()>3)
                    qobject_cast<QWidget*>(this->parent())->close();
            p = mouseEvent->pos();
    }
    return false;
}
