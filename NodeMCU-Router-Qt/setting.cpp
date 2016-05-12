
#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QThread>

/*****************处理UI界面各种按钮的槽函数***************************/

/* 服务器连接 */
void Widget::on_Connect_toolButton_clicked()
{
	ui->IP_lineEdit->setDisabled(true);
	ui->Connect_toolButton->setDisabled(true);
	ui->Disconnect_toolButton->setEnabled(true);
	ipAdress = ui->IP_lineEdit->text();
	ServerUrl = "http://" + ipAdress + "/index.php/Token/machine/";
}
/* 服务器断开 */
void Widget::on_Disconnect_toolButton_clicked()
{
	ui->IP_lineEdit->setEnabled(true);
	ui->Disconnect_toolButton->setDisabled(true);
	ui->Connect_toolButton->setEnabled(true);
}


void Widget::on_Clear_toolButton_clicked()
{
    ui->textEdit->clear();
}

/* 链接串口，设置WiFi */
void Widget::on_YespushButton_clicked()
{
    //定义串口对象，指定串口名和查询模式，这里使用事件驱动EventDriven
    //10以上的串口打开方法
    //Com = new Win_QextSerialPort("\\\\.\\COM10", QextSerialBase::EventDriven);
    //"\\\\.\\com10"是标准设备名称，自然是可疑打开的，而直接用com10则经过库函数转换成标准名称才用于设备操作，库函数没考虑超过十个串口，所以出问题了。
    MyCom = new Win_QextSerialPort(ui->ComcomboBox->currentText(),QextSerialBase::EventDriven);
    //myCom = new Win_QextSerialPort("\\\\.\\COM20",QextSerialBase::EventDriven);
    bool IsOpen = MyCom->open(QIODevice::ReadWrite);
    if(IsOpen)
    {
	//QMessageBox::information(this, tr("打开成功"), tr("成功打开串口！"), QMessageBox::Ok);
	//清除缓冲区
	MyCom->flush();
	char c_btl = ui->BotecomboBox->currentIndex();
	//波特率设置，我们设置
	switch(c_btl)
	{
	     case 0: MyCom->setBaudRate(BAUD9600);break;
	     case 1: MyCom->setBaudRate(BAUD38400);break;
	     case 2: MyCom->setBaudRate(BAUD115200);break;
	     default : break;
	}
	//数据位设置，我们设置为8位数据位
	MyCom->setDataBits(DATA_8);
	//奇偶校验设置，我们设置为无校验
	MyCom->setParity(PAR_NONE);
	//停止位设置，我们设置为1位停止位
	MyCom->setStopBits(STOP_1);
	//数据流控制设置，我们设置为无数据流控制
	MyCom->setFlowControl(FLOW_OFF);
	//延时设置，我们设置为延时500ms,这个在Windows下好像不起作用
	MyCom->setTimeout(500);

	ui->YespushButton->setDisabled(true);
	connect(MyCom,SIGNAL(readyRead()),this,SLOT(ReadCom()));



	QString ssid = ui->lineEdit_4->text();
	QString pwd = ui->lineEdit_5->text();

	/* wifi.sta.config("ssid","pwd") */
	QString wifi = "wifi.sta.config(\"" + ssid + "\",\"" + pwd + "\")\n";

	QByteArray Senddata = wifi.toUtf8();
	MyCom->write(Senddata);
	QThread::msleep(500);

	/* file.open("ip.lua","w+") */
	QByteArray open_file("file.open(\"ip.lua\",\"w+\")\n");
	MyCom->write(open_file);
	QThread::msleep(500);

	/* file.write('192.168.1.106') */
	QString ip = ui->lineEdit_3->text();
	QString Swrite_file = "file.write('" + ip + "')\n";
	QByteArray write_file = Swrite_file.toUtf8();
	MyCom->write(write_file);

	/* file.close() */
	QThread::msleep(500);
	QByteArray close_file("file.close()\n");
	MyCom->write(close_file);

	QMessageBox::warning(this,tr("成功"), tr("设置成功！"), QMessageBox::Ok);

	/* 关闭串口 */
	QThread::msleep(1000);
	MyCom->close();
	delete MyCom;
	//ui->YespushButton->setDisabled(false);

    }
    else
    {
	QMessageBox::warning(this,tr("打开失败"), tr("打开串口失败！"), QMessageBox::Ok);
	delete MyCom;
	return;
    }
}



void Widget::ReadCom()//读取数据
{
    //读取串口缓冲区的所有数据给临时变量temp
    QByteArray temp = MyCom->readAll();

    QString S = QString::fromUtf8(temp.data());

    ui->textEdit->insertPlainText(S);

}
