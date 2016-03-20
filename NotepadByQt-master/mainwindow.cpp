#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    textEdit = new QPlainTextEdit(this);
    setCentralWidget(textEdit);//把textEdit设置为主窗口的中央窗体
    resize(600,500);//设置窗口打开时的默认大小

     createActions();
     createMenus();
     createToolBar();

     setCurrentFile("");
     statusBar()->showMessage(tr("Ready"));//在状态栏上显示

     connect(textEdit->document(), SIGNAL(contentsChanged()),
             this, SLOT(documentWasModified()));

}

void MainWindow::documentWasModified()
{
    setWindowModified(textEdit->document()->isModified());
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("This Software writed by CXD"));
}

bool MainWindow::save()
{
    if (curFile.isEmpty())
    {
        QFileDialog dialog(this);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.exec();
        QStringList files = dialog.selectedFiles();

        if (files.isEmpty())
            return false;
        return saveFile(files.at(0));
    }
    else
    {
        return saveFile(curFile);
    }
}

void MainWindow::open()
{
    if(maybeSave())//判断文件是否改动过
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

void MainWindow::newfile()
{
    if (maybeSave())//判断文件是否改动过
    {
        textEdit->clear();
        setCurrentFile("");//设置文件名
    }
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))//
    {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out << textEdit->toPlainText();//将字符保存在文件中
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 5000);//在状态栏上显示5秒
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
    textEdit->setPlainText(in.readAll());//将文件读出来
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);//将文件名显示到窗口标题栏上
}

bool MainWindow::maybeSave()
{
    if (textEdit->document()->isModified()) //判断文件是否改动过
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);//设置快捷键
    newAct->setStatusTip(tr("Create a new file"));//设定状态条显示，当鼠标移至此选项时在状态条上显示
    connect(newAct, SIGNAL(triggered()), this, SLOT(newfile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    cutAct =new QAction(QIcon(":/images/cut.png"),tr("&cut"),this);
    cutAct->setShortcuts((QKeySequence::Cut));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct =new QAction(QIcon(":/images/copy.png"),tr("&copy"),this);
    copyAct->setShortcuts((QKeySequence::Copy));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct =new QAction(QIcon(":/images/paste.png"),tr("&paste"),this);
    pasteAct->setShortcuts((QKeySequence::Paste));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    aboutAct= new QAction(tr("about"),this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct,SIGNAL(triggered()),this,SLOT(about()));
}

void MainWindow::createToolBar()
{
    ToolBar = addToolBar(tr("ToolBar"));
    ToolBar->addAction(newAct);
    ToolBar->addAction(openAct);
    ToolBar->addAction(saveAct);
    ToolBar->addAction(cutAct);
    ToolBar->addAction(copyAct);
    ToolBar->addAction(pasteAct);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);

    fileMenu = menuBar()->addMenu(tr("&Edit"));
    fileMenu->addAction(cutAct);
    fileMenu->addAction(copyAct);
    fileMenu->addAction(pasteAct);

    fileMenu = menuBar()->addMenu(tr("&About"));
    fileMenu->addAction(aboutAct);
}

MainWindow::~MainWindow()
{

}
