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
