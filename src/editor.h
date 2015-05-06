#ifndef EDITOR_H
#define EDITOR_H

#include <QtGui>
#include <QTextEdit>
#include <QAction>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QFileInfo>
#include <QApplication>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include "parse.h"
#include "pubconst.h"
#include <string>

class Editor : public QTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget *parent = 0);

    void newFile();
    bool save();
    bool saveAs();
    bool exportHtml();
    void print();
    void printHtml();
    void insertFrag(const QString &str, int offset);
    QString getHtml();
    QAction *windowMenuAction() const {return action;}

    static Editor *open(QWidget *parent = 0);
    static Editor *openFile(const QString &fileName,
                            QWidget *parent = 0);
    bool okToContinue();

    void updateCSS(const std::string &_css);
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void documentWasModified();

private:
    bool saveFile(const QString &fileName);
    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void setCurrentFile(const QString &fileName);

    std::string css;
    QString curFile;
    bool isUntitled;
    QAction * action;

};

#endif // EDITOR_H
