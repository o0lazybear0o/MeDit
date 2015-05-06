#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtWidgets/QApplication>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QDate>
#include <QTime>
#include <QAction>
#include <QTextEdit>
#include <QClipboard>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QFileInfo>
#include <QStatusBar>
#include <QCloseEvent>
#include <QSettings>
#include <QWebView>
#include <QSplitter>
#include <QApplication>
#include <string>
#include "editor.h"
#include "parse.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent *event);
public slots:
    void openFile(const QString &fileName);
private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void loadFiles();
    void cut();
    void copy();
    void paste();
    void selectAll();
    void redo();
    void undo();
    void updateActions();
    void updateView();
    bool exportHtml();
    void printHtml();
    void print();
    void setCssClearness();
    void setCssClearnessDark();
    void setCssGitHub();
    void setCssGitHub2();
    void setCssCustom();
    void insertDate();
    void insertTime();
//    void strong();
//    void emphaSize();
//    void inlineCode();
    //void openRecentFile();

private:
    QAction * newAction;
    QAction * openAction;
    QAction * saveAction;
    QAction * saveAsAction;
    QAction * exportHtmlAction;
    QAction * printAction;
    QAction * printHtmlAction;
    QAction * exitAction;
    QAction * cutAction;
    QAction * copyAction;
    QAction * pasteAction;
    QAction * selectAllAction;
    QAction * redoAction;
    QAction * undoAction;
    QAction * copyHtmlAction;
    QAction * insertDateAction;
    QAction * insertTimeAction;
//    QAction * strongAction;
//    QAction * emphaSizeAction;
//    QAction * inlineCodeAction;
    QAction * closeAction;
    QAction * closeAllAction;
    QAction * nextAction;
    QAction * previousAction;
    QAction * aboutAction;
    QActionGroup * windowActionGroup;
    QAction * cssClearnessAction;
    QAction * cssClearnessDarkAction;
    QAction * cssGitHubAction;
    QAction * cssGitHub2Action;
    QAction * cssCustomAction;
    QActionGroup * cssActionGroup;

    //QAction * recentFileActions[MaxRecentFiles];
    QAction * separatorAction;

    QMenu * fileMenu;
    QMenu * editMenu;
    QMenu * cssSubMenu;
    QMenu * insertSubMenu;
    QMenu * actionMenu;
    QMenu * windowMenu;
    QMenu * helpMenu;

    QLabel * statusLabel;

    std::string css;

    void createActions();
    void createMenus();
    void createContextMenu();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    void addEditor(Editor *editor);
    void setCSS(const QString & fileName);
    //void updateRecentFileActions();

    Editor *activeEditor();
public:
    QMdiArea * mdiArea;
    QSplitter * splitter;
    QWebView * webView;
    QTextEdit * textEdit;

    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
