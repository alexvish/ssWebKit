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
    delete glWidget;
}


void WebkitWidget::initUI() {
    setRenderHint(QPainter::HighQualityAntialiasing);
    setRenderHint(QPainter::TextAntialiasing);
    glWidget = new QGLWidget();
    setViewport(glWidget);
    QSettings settings("QT","ssWebKit");
    QString urlString = settings.value("url","qrc://res/web-content/html/slides.html").toString();
    QUrl url = QUrl(urlString);

    layout = new QGridLayout(this);
    layout->setContentsMargins(0,0,0,0);

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
