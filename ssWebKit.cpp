#include "webkitwidget.h"
#include "screensaver.h"

int main(int argc, char* argv[]) {

    QApplication app(argc,argv);
    QStringList args = app.arguments();


    enum Mode{
        TEST,
        Preview,
        Config,
        Show
    };

    Mode mode = TEST;
    bool noErrors = (args.size() == 1);
    WId preview_parent_WId = 0;

    for(int i = 1, arglen = args.size(); i < arglen; i++) {
        if (args[i].size() < 2) {
            //assuming: noErrors = false;
            break;
        }
        switch(args[i][1].toLatin1()) {
            case 'c':
            case 'C':
                mode = Config;
                noErrors=true;
                break;
            case 's':
            case 'S':
                mode = Show;
                noErrors = true;
                break;
            case 'p':
            case 'P':
                mode = Preview;
                if(args[i].size()>=4 && args[i][2]==':') {
                        preview_parent_WId = (WId)args[i].mid(3).toUInt(&noErrors);
                } else if(args.size()>i) {
                    preview_parent_WId = (WId)args[i+1].toUInt(&noErrors);
                }
                break;
        }
    }
    if (!noErrors) {
        QMessageBox::critical(0,"ScreenSaver Error","Could not parse arguments:\n"
                              + args.join("\n")
                              +"\n Valid args are:\n"
                              + "    /S - screensaver mode\n"
                              + "    /P windowPid| /P:windowPid - preview mode\n"
                              + "    /C - config mode\n");
        return -1;
    }

    ScreenSaver* ss = 0;

    WebkitWidget* w = new WebkitWidget();
    w->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(w,SIGNAL(destroyed()), &app, SLOT(quit()));


    switch(mode) {
        case TEST:
            ss = new ScreenSaver(&app);
            break;
        case Show:
            ss = new ScreenSaver(&app);
            ss->closeOnMouseAndKeyboardEvents();
            break;
        case Preview:
            return 0;
            //something is really wrong with preview
            //ss = new ScreenSaver(&app,preview_parent_WId);
            //break;
        case Config:
            QSettings settings("QT","ssWebKit");
            QString urlString = settings.value("url","qrc://res/web-content/html/slides.html").toString();
            urlString = QInputDialog::getText(0,"Configure Webkit Screensaver","Set url", QLineEdit::Normal, urlString);
            if (urlString != 0 && urlString.size()>0) {
                settings.setValue("url",urlString);
            }
            return 0;

    }

    int result = app.exec();
    if (ss) {
        delete ss;
    }
    return result;
}
