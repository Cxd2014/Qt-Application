#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QtSql/QSqlDatabase>

#include <QtSql/QSqlQuery>

#include <QString>
#include <QTextEdit>
#include <QPalette>
#include <QMessageBox>
#include "win_qextserialport.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    //void StringToHex(QString str,QByteArray &senddata);//进制转换函数
    //char ConvertHexChar(char ch);
    void Del_String(QString str);//处理串口发送的数据
    void Update_Database(QString str1,QString str2);//每次消费和充值都要更新数据库

private slots:
    void on_SqlConnectButton_clicked();

    void on_CmdpushButton_clicked();

    void on_ClearpushButton_clicked();

    void on_SqlClosepushButton_clicked();

    void on_LogIn_OKpushButton_clicked();

    void on_Log_LogOutpushButton_clicked();

    void on_RFID_OpenpushButton_clicked();

    void ReadCom();//读取数据

   // void on_RFID_SendpushButton_clicked();

   // void on_RFID_CmdpushButton_clicked();

    void on_RFID_ClosepushButton_clicked();

private:

    Ui::Widget *ui;
    Win_QextSerialPort *MyCom;//声明串口对象
};

#endif // WIDGET_H
