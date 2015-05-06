#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mdiArea = new QMdiArea;

    mdiArea->setTabsClosable(true);
    mdiArea->setTabsMovable(true);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setTabPosition(QTabWidget::North);
    mdiArea->setTabShape(QTabWidget::Triangular);

    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateActions()));

    webView = new QWebView;

    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(mdiArea);
    splitter->addWidget(webView);
    setCentralWidget(splitter);

    createActions();
    createMenus();
    createStatusBar();

    //readSettings();
    setWindowIcon(QPixmap(":/img/icon.png"));
    setWindowTitle(tr("MeDit"));

    setCssClearness();

    QTimer::singleShot(0, this, SLOT(loadFiles()));
    //QTimer::singleShot(0, this, SLOT(loadFiles()));
}

void MainWindow::openFile(const QString &fileName)
{
    Editor *editor = Editor::openFile(fileName,this);
    if(editor)
        addEditor(editor);
}

void MainWindow::loadFiles()
{
    QStringList args = QApplication::arguments();
    args.removeFirst();
    if (!args.isEmpty())
    {
        foreach (QString arg, args) {
            openFile(arg);
        }
        //mdiArea->cascadeSubWindows();
    } else {
        newFile();
    }
    mdiArea ->activateNextSubWindow();
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

//    for (int i = 0; i < MaxRecentFiles; ++i)
//    {
//        recentFileActions[i] = new Action(this);
//        recentFileActions[i] ->setVisible(false);
//        connect(recentFileActions[i], SIGNAL(triggered()),
//                this, SLOT(openRecentFile));
//    }

    exportHtmlAction = new QAction(tr("&Export Html"), this);
    connect(exportHtmlAction, SIGNAL(triggered()), this, SLOT(exportHtml()));

    printAction = new QAction(tr("&Print..."), this);
    printAction ->setShortcut(QKeySequence::Print);
    connect(printAction, SIGNAL(triggered()), this, SLOT(print()));

    printHtmlAction = new QAction(tr("Print &Html..."), this);
    connect(printHtmlAction, SIGNAL(triggered()), this, SLOT(printHtml()));


    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl + Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    redoAction = new QAction(tr("&Redo"), this);
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));


    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));


    //cutAction
    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    //copyAction
    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    //pasteAction
    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    selectAllAction = new QAction(tr("Select &All"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));

    cssActionGroup = new QActionGroup(this);

    cssClearnessAction = new QAction(tr("&Clearness"), this);
    connect(cssClearnessAction, SIGNAL(triggered()), this, SLOT(setCssClearness()));

    cssClearnessDarkAction = new QAction(tr("Clearness&Dark"), this);
    connect(cssClearnessDarkAction, SIGNAL(triggered()), this, SLOT(setCssClearnessDark()));

    cssGitHubAction = new QAction(tr("&GitHub"), this);
    connect(cssGitHubAction, SIGNAL(triggered()), this, SLOT(setCssGitHub()));

    cssGitHub2Action = new QAction(tr("Git&Hub2"), this);
    connect(cssGitHub2Action, SIGNAL(triggered()), this, SLOT(setCssGitHub2()));

    cssCustomAction = new QAction(tr("Custom ..."), this);
    connect(cssCustomAction, SIGNAL(triggered()), this, SLOT(setCssCustom()));

    insertDateAction = new QAction(tr("Current Date"), this);
    connect(insertDateAction, SIGNAL(triggered()), this, SLOT(insertDate()));

    insertTimeAction = new QAction(tr("Current Time"), this);
    connect(insertTimeAction, SIGNAL(triggered()), this, SLOT(insertTime()));

    //closeAction
    closeAction = new QAction(tr("Cl&ose"), this);
    closeAction->setShortcut(QKeySequence::Close);
    connect(closeAction, SIGNAL(triggered()),mdiArea, SLOT(closeActiveSubWindow()));

    //closeAllAction
    closeAllAction = new QAction(tr("Close &All"), this);
    connect(closeAllAction, SIGNAL(triggered()), this, SLOT(close()));

    //newAction
    nextAction = new QAction(tr("Ne&xt"), this);
    nextAction->setShortcut(QKeySequence::NextChild);
    connect(nextAction, SIGNAL(triggered()),mdiArea, SLOT(activateNextSubWindow()));

    //previousAction
    previousAction = new QAction(tr("Pre&vious"), this);
    previousAction->setShortcut(QKeySequence::PreviousChild);
    connect(previousAction, SIGNAL(triggered()),mdiArea, SLOT(activatePreviousSubWindow()));

    windowActionGroup = new QActionGroup(this);

    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

}

void MainWindow::createMenus()
{
    fileMenu = QMainWindow::menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu ->addAction(openAction);
    separatorAction = fileMenu->addSeparator();
    fileMenu ->addAction(saveAction);
    fileMenu -> addAction(saveAsAction);
//    for (int i = 0; i < MaxRecentFiles; ++i)
//        fileMenu->addAction(recentFileActions[i]);
    fileMenu ->addAction(exportHtmlAction);
    fileMenu->addSeparator();
    fileMenu ->addAction(printAction);
    fileMenu ->addAction(printHtmlAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = QMainWindow::menuBar()->addMenu(tr("&Edit"));
    editMenu ->addAction(redoAction);
    editMenu->addAction(undoAction);
    editMenu ->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);

    editMenu ->addSeparator();

    //cssSubMenu

    cssActionGroup->addAction(cssClearnessAction);
    cssActionGroup->addAction(cssClearnessDarkAction);
    cssActionGroup->addAction(cssGitHubAction);
    cssActionGroup->addAction(cssGitHub2Action);
    cssActionGroup->addAction(cssCustomAction);
    cssClearnessAction->setChecked(true);
    cssSubMenu = editMenu->addMenu(tr("C&SS"));
    cssSubMenu->addAction(cssClearnessAction);
    cssSubMenu->addAction(cssClearnessDarkAction);
    cssSubMenu->addAction(cssGitHubAction);
    cssSubMenu->addAction(cssGitHub2Action);
    cssSubMenu->addAction(cssCustomAction);

    actionMenu = QMainWindow::menuBar()->addMenu(tr("&Actions"));
    insertSubMenu =  actionMenu->addMenu(tr("&Insert"));
    insertSubMenu->addAction(insertDateAction);
    insertSubMenu->addAction(insertTimeAction);
    actionMenu->addAction(selectAllAction);

    windowMenu = QMainWindow::menuBar()->addMenu(tr("&Windows"));
    windowMenu->addAction(closeAction);
    windowMenu->addAction(closeAllAction);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAction);
    windowMenu->addAction(previousAction);
    windowMenu->addAction(separatorAction);


    helpMenu = QMainWindow::menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);

}

//Not completed;
void MainWindow::createStatusBar()
{
    statusLabel = new QLabel("Ready");
    statusBar()->addWidget(statusLabel);
}


void MainWindow::addEditor(Editor *editor)
{
    connect(editor, SIGNAL(copyAvailable(bool)), cutAction, SLOT(setEnabled(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)));

    QMdiSubWindow *subWindow = mdiArea->addSubWindow(editor);
    windowMenu->addAction(editor->windowMenuAction());
    windowActionGroup ->addAction(editor->windowMenuAction());

    editor->updateCSS(css);

    connect(editor, SIGNAL(textChanged()), this, SLOT(updateView()));

    subWindow->show();
}

void MainWindow::newFile()
{
    Editor *editor = new Editor;
    editor->newFile();
    addEditor(editor);
}

void MainWindow::open()
{
    Editor *editor = Editor::open(this);
    if (editor)
        addEditor(editor);
}



bool MainWindow::saveAs()
{
    if (activeEditor())
        activeEditor()->saveAs();
}

bool MainWindow::save()
{
    if (activeEditor())
        activeEditor()->save();
}

bool MainWindow::exportHtml()
{
    if (activeEditor())
        activeEditor()->exportHtml();
}

void MainWindow::print()
{
    if (activeEditor())
        activeEditor()->print();
}

void MainWindow::printHtml()
{
    if (activeEditor())
        activeEditor()->printHtml();
}

//overwirted
//if the MainWindow get the close() SLOT
//ask if ok to Continue first
void MainWindow::closeEvent(QCloseEvent * event)
{

}


void MainWindow::cut()
{
    if(activeEditor())
        activeEditor()->cut();
}

void MainWindow::copy()
{
    if(activeEditor())
        activeEditor()->copy();
}

void MainWindow::paste()
{
    if(activeEditor())
        activeEditor()->paste();
}

void MainWindow::selectAll()
{
    if (activeEditor())
        activeEditor() ->selectAll();
}

void MainWindow::redo()
{
    if (activeEditor())
        activeEditor()->redo();
}

void MainWindow::undo()
{
    if (activeEditor())
        activeEditor()->undo();
}

void MainWindow::setCSS(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this,tr("MeDit"),tr("Cannot read file %1:\n%2.").
                             arg(file.fileName()).arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString str = in.readAll();
    css = str.toStdString();
    QApplication::restoreOverrideCursor();
    updateActions();
}

void MainWindow::setCssClearness()
{
    setCSS(CSSClearness);
}

void MainWindow::setCssClearnessDark()
{
    setCSS(CSSClearnessDark);
}

void MainWindow::setCssGitHub()
{
    setCSS(CSSGitHub);
}

void MainWindow::setCssGitHub2()
{
    setCSS(CSSGitHub2);
}

void MainWindow::setCssCustom()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open"), ".");
    if (fileName.isEmpty())
        return;
    setCSS(fileName);
}

void MainWindow::insertDate()
{
    if (activeEditor())
    {
        QString date = QDate::currentDate().toString();
        activeEditor()->insertPlainText(date);
    }
}

void MainWindow::insertTime()
{
    if (activeEditor())
    {
        QString time = QTime::currentTime().toString();
        activeEditor()->insertPlainText(time);
    }
}

//void MainWindow::strong()
//{
//    if (activeEditor())
//    {
//        activeEditor()->insertFrag("****", 2);
//    }
//}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About MeDit"),
                       tr("<h2> MeDit 1.0 </h2>"
                          "<p>Copyright &copy Iris CHEN"
                          "<p>MeDit is a Markdown Editor in Linux."));
}

void MainWindow::writeSettings()
{
    QSettings settings("Iris CHEN", "MeDitTabs");

    //settings.setValue("recentFiles", recentFiles);
}

void MainWindow::readSettings()
{
    QSettings settings("Iris CHEN", "MeDitTabs");

    //recentFiles = settings.value("recentFiles").toStringList();
    //updateRecentFileActions();

}

Editor *MainWindow::activeEditor()
{
    QMdiSubWindow *subWindow = mdiArea->activeSubWindow();
    if(subWindow)
        return qobject_cast<Editor *>(subWindow->widget());
    return 0;
}

void MainWindow::updateView()
{
    if (activeEditor())
    {
       QString html = activeEditor()->getHtml();
       webView->setHtml(html);
    }
}

void MainWindow::updateActions()
{
    bool hasEditor = (activeEditor()!=0);
    bool hasSelection = activeEditor() && activeEditor()->textCursor().hasSelection();

    saveAction->setEnabled(hasEditor);
    saveAsAction->setEnabled(hasEditor);
    printAction->setEnabled(hasEditor);
    printHtmlAction->setEnabled(hasEditor);
    exportHtmlAction->setEnabled(hasEditor);
    cutAction->setEnabled(hasSelection);
    copyAction->setEnabled(hasSelection);
    pasteAction->setEnabled(hasEditor);
    selectAllAction->setEnabled(hasEditor);
    redoAction ->setEnabled(hasEditor);
    undoAction ->setEnabled(hasEditor);
    closeAction->setEnabled(hasEditor);
    closeAllAction->setEnabled(hasEditor);
    nextAction->setEnabled(hasEditor);
    previousAction->setEnabled(hasEditor);
    separatorAction->setEnabled(hasEditor);

    if(activeEditor())
        activeEditor()->windowMenuAction()->setChecked(true);  //菜单栏可点击
    if (activeEditor())
        activeEditor()->updateCSS(css);

    updateView();
}

MainWindow::~MainWindow()
{

}
