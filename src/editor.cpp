#include "editor.h"

Editor::Editor(QWidget *parent)
    : QTextEdit(parent)
{
    action = new QAction(this);
    action->setCheckable(true);
    connect(action, SIGNAL(triggered()), this, SLOT(show()));
    connect(action, SIGNAL(triggered()), this, SLOT(setFocus()));

    isUntitled = true;

    connect(document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));

    setWindowTitle("[*]");
    setAttribute(Qt::WA_DeleteOnClose);
}

void Editor::insertFrag(const QString &str, int offset)
{
//    int pos = this->cursor();
//    pos += offset;
//    insertPlainText(str);
//    this->setCursor(pos);
}

void Editor::documentWasModified()
{
    setWindowModified(true);
}

void Editor::newFile()
{
    static int documentNumber = 1;
    curFile = tr("document%1.md").arg(documentNumber);
    setWindowTitle(curFile + "[*]");
    isUntitled = true;
    ++documentNumber;
}

Editor * Editor::open(QWidget *parent)
{
    QString fileName =
            QFileDialog::getOpenFileName(parent, tr("Open"), ".");
    if (fileName.isEmpty())
        return 0;
    return openFile(fileName, parent);
}

Editor * Editor::openFile(const QString &fileName, QWidget *parent)
{
    Editor * editor = new Editor(parent);

    if (editor->readFile(fileName))
    {
        editor->setCurrentFile(fileName);
        editor->isUntitled = false;
        return editor;
    } else {
        delete editor;
        return 0;
    }
}

bool Editor::save()
{
    if (isUntitled)
    {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool Editor::okToContinue()
{
    if (isWindowModified()){
        int r = QMessageBox::warning(this, tr("MeDit"),
                                     tr("The document- has been modified.\n"
                                        "Do you want to save your changes?"),
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (r == QMessageBox::Yes)
        {
            return save();
        } else if (r == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}


void Editor::closeEvent(QCloseEvent *event)
{
    if (okToContinue())
    {
        event->accept();
    } else {
        event->ignore();
    }
}


bool Editor::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Markdown File"),".",
                                                       tr("Markdown File(*.md)"));
    if (fileName.isEmpty())
        return false;
    return saveFile(fileName);
}

//uncompleted
bool Editor::saveFile(const QString &fileName)
{
    if (writeFile(fileName))
    {
        setCurrentFile(fileName);
        return true;
    }
    else
        return false;
}

QString Editor::getHtml()
{
    QString html;
    QString content = toPlainText();
    if (content.isEmpty())
        html = "";
    else
    {
        std::string str = content.toStdString();
        str = tofinalhtml(css, str);
        html = QString::fromStdString(str);
    }
    return html;
}

bool Editor::exportHtml()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Markdown File"),".",
                                                       tr("Html(*.html)"));
    if (fileName.isEmpty())
        return false;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this,tr("MeDit"),
                             tr("Cannot write file %1:\n%2").arg(file.fileName()).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << getHtml();
    QApplication::restoreOverrideCursor();
    return true;
}

bool Editor::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this,tr("MeDit"),
                             tr("Cannot write file %1:\n%2").arg(file.fileName()).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();
    QApplication::restoreOverrideCursor();
    return true;
}


bool Editor::readFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this,tr("MeDit"),tr("Cannot read file %1:\n%2.").
                             arg(file.fileName()).arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    return true;
}

void Editor::print()
{
    QPrinter * printer = new QPrinter(QPrinter::PrinterResolution);
    QPrintDialog * printDialog = new QPrintDialog(printer, this);
    if (printDialog->exec())
    {
        QTextDocument * doc = this->document();
        doc->print(printer);
    }
}

void Editor::printHtml()
{
    QPrinter * printer = new QPrinter(QPrinter::PrinterResolution);
    QPrintDialog * printDialog = new QPrintDialog(printer, this);
    if (printDialog->exec())
    {
        QTextDocument * doc = new QTextDocument();
        doc->setHtml(getHtml());
        doc->print(printer);
    }

}

void Editor::updateCSS(const std::string &_css)
{
    css = _css;
}

QString Editor::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


void Editor::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    isUntitled = false;
    action -> setText(strippedName(curFile));
    document()->setModified(false);
    setWindowTitle(strippedName(curFile) + "[*]");
    setWindowModified(false);
}

