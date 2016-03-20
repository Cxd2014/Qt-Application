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
    void documentWasModified();//�ж��ļ��Ƿ��޸�

private:
    void createMenus();//��Ӳ˵���
    void createActions();//��Ӳ˵������ѡ��
    void createToolBar();//������ݹ�����
    bool maybeSave();    //�ж��ļ��Ƿ�Ķ���������Ķ��˱�ѯ���Ƿ���Ҫ����
    void setCurrentFile(const QString &fileName);//���õ�ǰ�ļ����ļ���
    bool saveFile(const QString &fileName);//�����ļ�
    void loadFile(const QString &fileName);//��ȡ�ļ�����


    QPlainTextEdit *textEdit;//�����ı��༭������
    QString curFile;//�����ļ����ļ���

    QMenu *fileMenu;//�����˵���
    QAction *newAct;//�����˵������ѡ��
    QAction *openAct;
    QAction *saveAct;

    QMenu *editMenu;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;

    QMenu *aboutMenu;
    QAction *aboutAct;

    QToolBar *ToolBar;//������ݹ�����

};

#endif // MAINWINDOW_H
