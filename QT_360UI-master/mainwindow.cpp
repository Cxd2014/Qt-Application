#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//隐藏窗口框
    this->setWindowTitle("360安全卫士");//设置软件标题

    /* 显示菜单 */
    QMenu * menu = ui->AboutpushButton->getmenu();
    SettingAct = new QAction(QIcon(":/images/setting.png"), tr("&设置"), this);
    AboutAct = new QAction(QIcon(":/images/about.png"), tr("&关于我们"), this);
    menu->addAction(SettingAct);
    menu->addAction(AboutAct);
    connect(AboutAct,SIGNAL(triggered()),this,SLOT(AboutMessageBox()));


}

void MainWindow::AboutMessageBox()
{
    QMessageBox::information(this,tr("山寨360"),tr("山寨版360 BY CXD"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_closeToolButton_clicked()
{
    /* 慢慢消失的效果 */
    QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();

    connect(animation,SIGNAL(finished()),this,SLOT(close()));//效果显示完后关闭

}

void MainWindow::on_MinitoolButton_clicked()
{
    this->showMinimized();//最小化
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
	//记录窗口此时的坐标
	StartPos = event->globalPos() - this->frameGeometry().topLeft();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint EndPos;
    if ( event->buttons() == Qt::LeftButton )
    {
	EndPos = event->globalPos() - StartPos;
	this->move(EndPos);
    }
}

void MainWindow::setStackCurrentPage(quint8 index)
{
    QWidget *widget = ui->stackedWidget->widget(index);
    ui->stackedWidget->setCurrentWidget(widget);
}

void MainWindow::on_ChecktoolButton_clicked()
{
    this->setStackCurrentPage(0);
}

void MainWindow::on_KilltoolButton_clicked()
{
    this->setStackCurrentPage(1);
}

void MainWindow::on_FixtoolButton_clicked()
{
    this->setStackCurrentPage(2);
}

void MainWindow::on_CleantoolButton_clicked()
{
    this->setStackCurrentPage(3);
}

void MainWindow::on_SpeedtoolButton_clicked()
{
    this->setStackCurrentPage(4);
}

void MainWindow::on_ComputertoolButton_clicked()
{
    this->setStackCurrentPage(5);
}

void MainWindow::on_SofttoolButton_clicked()
{
    this->setStackCurrentPage(6);
}
