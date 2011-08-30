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
#include "screensaver.h"

WId previewWId(QString s);

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
                    args[i].mid(3);
                    preview_parent_WId = previewWId(args[i].mid(3));
                } else if(args.size()>i) {
                    preview_parent_WId = previewWId(args[i+1]);
                    i++;
                } else {
                    preview_parent_WId = 0;
                }
                if (preview_parent_WId) {
                    noErrors = true;
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
            //return 0;
            //something is really wrong with preview
            ss = new ScreenSaver(&app,preview_parent_WId);
            break;
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

#include <tchar.h>

WId previewWId(QString s) {
    if (s == "Scrprev") {
        return (WId)FindWindow(L"Scrprev",NULL);
    } else {
        bool convertedOk;
        WId result = (WId)s.toUInt(&convertedOk);
        if (convertedOk) {
            return result;
        } else {
            return 0;
        }
    }
}

/*

HWND CheckForScrprev() {

  HWND hwnd=FindWindow(_T("Scrprev"),NULL); // looks for the Scrprev class
  if (hwnd==NULL) // try to load it
  {
    STARTUPINFO si; PROCESS_INFORMATION pi; ZeroMemory(&si,sizeof(si)); ZeroMemory(&pi,sizeof(pi));
    si.cb=sizeof(si);
    TCHAR cmd[MAX_PATH]; _tcscpy(cmd,_T("Scrprev")); // unicode CreateProcess requires it writeable
    BOOL cres=CreateProcess(NULL,cmd,0,0,FALSE,CREATE_NEW_PROCESS_GROUP | CREATE_DEFAULT_ERROR_MODE,
                            0,0,&si,&pi);
    if (!cres) {Debug(_T("Error creating scrprev process")); return NULL;}
    DWORD wres=WaitForInputIdle(pi.hProcess,2000);
    if (wres==WAIT_TIMEOUT) {Debug(_T("Scrprev never becomes idle")); return NULL;}
    if (wres==0xFFFFFFFF) {Debug(_T("ScrPrev, misc error after ScrPrev execution"));return NULL;}
    hwnd=FindWindow(_T("Scrprev"),NULL);
  }
  if (hwnd==NULL) {Debug(_T("Unable to find Scrprev window")); return NULL;}
  ::SetForegroundWindow(hwnd);
  hwnd=GetWindow(hwnd,GW_CHILD);
  if (hwnd==NULL) {Debug(_T("Couldn't find Scrprev child")); return NULL;}
  return hwnd;
}
*/
