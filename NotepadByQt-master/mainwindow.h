#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QPlainTextEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QToolBar>
#include <QTextStream>
#include <QApplication>
#include <QStatusBar>
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void about();
    void open();
    bool save();
    void newfile();
    void documentWasModified();//判断文件是否被修改

private:
    void createMenus();//添加菜单项
    void createActions();//添加菜单下面的选项
    void createToolBar();//创建快捷工具栏
    bool maybeSave();    //判断文件是否改动过，如果改动了便询问是否需要保存
    void setCurrentFile(const QString &fileName);//设置当前文件的文件名
    bool saveFile(const QString &fileName);//保存文件
    void loadFile(const QString &fileName);//读取文件内容


    QPlainTextEdit *textEdit;//定义文本编辑主窗口
    QString curFile;//保存文件得文件名

    QMenu *fileMenu;//创建菜单项
    QAction *newAct;//创建菜单下面的选项
    QAction *openAct;
    QAction *saveAct;

    QMenu *editMenu;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;

    QMenu *aboutMenu;
    QAction *aboutAct;

    QToolBar *ToolBar;//创建快捷工具栏

};

#endif // MAINWINDOW_H
