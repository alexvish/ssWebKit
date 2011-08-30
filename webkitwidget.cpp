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
#include <windows.h>


WebkitWidget::WebkitWidget()
    :QGraphicsView(),
     eventFilter(NULL)
{
    this->initUI();
}

WebkitWidget::WebkitWidget(WId parentWId)
    : QGraphicsView(),
      eventFilter(NULL){
    SetParent(this->winId(),parentWId);
    SetWindowLong(this->winId(),GWL_STYLE,WS_CHILDWINDOW|WS_VISIBLE);
    this->initUI();
}

WebkitWidget::~WebkitWidget() {
    if (eventFilter) {
        delete eventFilter;
    }
    destroy();
    //delete layout;
}



void WebkitWidget::initUI() {
    QSettings settings("QT","ssWebKit");
    QString urlString = settings.value("url","qrc://res/web-content/html/slides.html").toString();
    QUrl url = QUrl(urlString);

    scene = new QGraphicsScene();
    setScene(scene);
    //layout = new QGridLayout(this);
    //layout->setContentsMargins(0,0,0,0);


    view = new QGraphicsWebView();
    view->setFiltersChildEvents(true);
    view->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled,true);
    view->settings()->setAttribute(QWebSettings::AcceleratedCompositingEnabled,false);
    view->setResizesToContents(false);
    view->load(url);


    scene->addItem(view);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void WebkitWidget::closeOnMouseAndKeyboardEvents() {
    if (!eventFilter) {
        eventFilter = new EventFilter(this);
    }
}

void WebkitWidget::resizeEvent(QResizeEvent *event) {
    view->resize(width()/matrix().m11(),height()/matrix().m22());
    QGraphicsView::resizeEvent(event);
}


EventFilter::EventFilter (WebkitWidget* w)
    :QObject(w)
    ,p(-1,-1)
{
    w->installEventFilter(this);
    w->view->installEventFilter(this);
}

EventFilter::~EventFilter() {

}

bool EventFilter::eventFilter(QObject *obj, QEvent *event) {
    // quit if key is pressed or mouse is moved or pressed
    if (event->type() == QEvent::KeyPress
            || event->type() == QEvent::MouseButtonPress
            || event->type() == QEvent::GraphicsSceneMousePress
            || event->type() == QEvent::GraphicsSceneWheel
            || event->type() == QEvent::GraphicsSceneContextMenu
            || event->type() == QEvent::GraphicsSceneDragMove
            || event->type() == QEvent::GraphicsSceneDrop
            || event->type() == QEvent::GraphicsSceneHelp
            || event->type() == QEvent::GraphicsSceneMouseDoubleClick
            ) {


        qobject_cast<QWidget*>(this->parent())->close();
    }

    if( event->type()==QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(p != QPoint(-1, -1) && (p-mouseEvent->pos()).manhattanLength()>3)
                    qobject_cast<QWidget*>(this->parent())->close();
            p = mouseEvent->pos();
    }
    if (event->type()==QEvent::GraphicsSceneMouseMove) {
        QGraphicsSceneMouseEvent *gscMouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);

        if(p != QPoint(-1, -1) && (p-(gscMouseEvent->pos())).manhattanLength()>3)
                qobject_cast<QWidget*>(this->parent())->close();
        p = gscMouseEvent->pos().toPoint();
    }
    if (event->type()==QEvent::GraphicsSceneHoverMove) {
        QGraphicsSceneHoverEvent *gscHoverEvent = static_cast<QGraphicsSceneHoverEvent*>(event);

        if(p != QPoint(-1, -1) && (p-gscHoverEvent->pos().toPoint()).manhattanLength()>3)
                qobject_cast<QWidget*>(this->parent())->close();
        p = gscHoverEvent->pos().toPoint();
    }

    return false;
}
