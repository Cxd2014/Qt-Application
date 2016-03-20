#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

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

/* 串口打开 */
void Widget::on_Open_toolButton_clicked()
{
	myCom = new QextSerialPort("/dev/"+ui->COM_comboBox->currentText(),QextSerialPort::EventDriven);
	
	if(myCom->open(QIODevice::ReadWrite))
	{
		ui->Close_toolButton->setEnabled(true);
		ui->Open_toolButton->setDisabled(true);
		ui->Bote_comboBox->setDisabled(true);
		ui->COM_comboBox->setDisabled(true);
		
		QMessageBox::information(this, trUtf8("打开成功"), trUtf8("已经成功打开串口：")+ui->COM_comboBox->currentText(), QMessageBox::Ok);
	}
	else
	{
		QMessageBox::information(this, trUtf8("打开失败"), trUtf8("打开串口：")+ui->COM_comboBox->currentText()+trUtf8("失败！"), QMessageBox::Ok);
		return;
	}

	char c_btl = ui->Bote_comboBox->currentIndex();
	switch(c_btl)   //设置串口波特率
	{
		case 0: myCom->setBaudRate(BAUD9600);break;
		case 1: myCom->setBaudRate(BAUD38400);break;
		case 2: myCom->setBaudRate(BAUD115200);break;
		default : break;
	}

	myCom->setDataBits(DATA_8);
	//数据位设置，我们设置为8位数据位

	myCom->setParity(PAR_NONE);
	//奇偶校验设置，我们设置为无校验

	myCom->setStopBits(STOP_1);
	//停止位设置，我们设置为1位停止位

	myCom->setFlowControl(FLOW_OFF);
	//数据流控制设置，我们设置为无数据流控制

	myCom->setTimeout(1);
	//延时设置，我们设置为延时500ms,这个在Windows下好像不起作用
	
	connect(myCom,SIGNAL(readyRead()),this,SLOT(ReadCom())); 
	//当串口有数据时调用 ReadCom() 函数
}

/* 串口关闭 */
void Widget::on_Close_toolButton_clicked()
{
	QMessageBox::StandardButton replay;
	replay = QMessageBox::question(this,trUtf8("确认"), trUtf8("关闭串口?"),
			      QMessageBox::Yes|QMessageBox::No);
	if(replay == QMessageBox::Yes){
		ui->Open_toolButton->setEnabled(true);
		ui->Bote_comboBox->setEnabled(true);
		ui->COM_comboBox->setEnabled(true);
		ui->Close_toolButton->setDisabled(true);
		myCom->close();
	}
}

void Widget::on_Clear_toolButton_clicked()
{
	ui->textEdit->clear();
}

void Widget::on_toolButton_1_clicked()
{
	if(ui->IP_lineEdit->isEnabled())
		ui->IP_lineEdit->insert("1");
}

void Widget::on_toolButton_2_clicked()
{	
	if(ui->IP_lineEdit->isEnabled())
		ui->IP_lineEdit->insert("2");
}

void Widget::on_toolButton_3_clicked()
{
	if(ui->IP_lineEdit->isEnabled())
		ui->IP_lineEdit->insert("3");
}

void Widget::on_toolButton_4_clicked()
{
	if(ui->IP_lineEdit->isEnabled())
		ui->IP_lineEdit->insert("4");
}

void Widget::on_toolButton_5_clicked()
{
	if(ui->IP_lineEdit->isEnabled())
		ui->IP_lineEdit->insert("5");
}

void Widget::on_toolButton_6_clicked()
{
	if(ui->IP_lineEdit->isEnabled())
		ui->IP_lineEdit->insert("6");
}

void Widget::on_toolButton_7_clicked()
{
	if(ui->IP_lineEdit->isEnabled())
		ui->IP_lineEdit->insert("7");
}

void Widget::on_toolButton_8_clicked()
{
	if(ui->IP_lineEdit->isEnabled())
		ui->IP_lineEdit->insert("8");
}

void Widget::on_toolButton_9_clicked()
{	
	if(ui->IP_lineEdit->isEnabled())
		ui->IP_lineEdit->insert("9");
}

void Widget::on_toolButton_10_clicked()
{
	if(ui->IP_lineEdit->isEnabled())
		ui->IP_lineEdit->insert("0");
}


void Widget::on_toolButton_11_clicked()
{
	if(ui->IP_lineEdit->isEnabled())
		ui->IP_lineEdit->insert(".");
}

void Widget::on_toolButton_12_clicked()
{
	if(ui->IP_lineEdit->isEnabled()){
		QString ip;
		ip = ui->IP_lineEdit->text();
		int L = ip.length(); //得到字符串的长度
		ip = ip.mid(0,L-1);  //取出字符串的前 L-1 个字符
		ui->IP_lineEdit->setText(ip);
	}
}

void Widget::on_toolButton_13_clicked()
{
	if(ui->IP_lineEdit->isEnabled())
		ui->IP_lineEdit->insert(":");
}
