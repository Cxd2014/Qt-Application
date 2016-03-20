#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
//以下函数都是操作RFID界面的


//打开串口
void Widget::on_RFID_OpenpushButton_clicked()
{
    //定义串口对象，指定串口名和查询模式，这里使用事件驱动EventDriven
	//10以上的串口打开方法
	//Com = new Win_QextSerialPort("\\\\.\\COM10", QextSerialBase::EventDriven);
	//"\\\\.\\com10"是标准设备名称，自然是可疑打开的，而直接用com10则经过库函数转换成标准名称才用于设备操作，库函数没考虑超过十个串口，所以出问题了。
	MyCom = new Win_QextSerialPort(ui->RFID_ComcomboBox->currentText(),QextSerialBase::EventDriven);
	//myCom = new Win_QextSerialPort("\\\\.\\COM20",QextSerialBase::EventDriven);
	bool IsOpen = MyCom->open(QIODevice::ReadWrite);
	if(IsOpen)
	{
	    //QMessageBox::information(this, tr("打开成功"), tr("成功打开串口！"), QMessageBox::Ok);
	    //清除缓冲区
	    MyCom->flush();
	    char c_btl = ui->RFID_BotecomboBox->currentIndex();
	    //波特率设置，我们设置
	    switch(c_btl)
	    {
		 case 0: MyCom->setBaudRate(BAUD9600);break;
		 case 1: MyCom->setBaudRate(BAUD115200);break;
		 case 2: MyCom->setBaudRate(BAUD38400);break;
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

	    ui->RFID_ClosepushButton->setVisible(true);
	    ui->RFID_OpenpushButton->setVisible(false);

	    connect(MyCom,SIGNAL(readyRead()),this,SLOT(ReadCom()));
	}
	else
	{
	    QMessageBox::warning(this,tr("打开失败"), tr("打开串口失败！"), QMessageBox::Ok);
	    delete MyCom;
	    return;
	}
}



//处理串口发送的数据并显示出来
void Widget::Del_String(QString str)
{
    ui->Log_RFIDlineEdit->clear();
    ui->Log_MonylineEdit->clear();
    ui->Log_NamelineEdit->clear();
    ui->Log_SFIDlineEdit->clear();
    ui->Log_NumlineEdit->clear();
    ui->Log_RoomlineEdit->clear();

    int Money[6];
    QString RFID = str.mid(0,8);//取出前八个数据
    ui->Log_RFIDlineEdit->insert(RFID);

    bool ok;
    for(int i = 0;i<6;i++)
	Money[i] = str.mid(8+i,1).toInt(&ok,16);//将16进制的字符串转换分离出来

    double temp = 0;//转换为十进制数
    temp = temp+Money[0]*4096;
    temp = temp+Money[1]*256;
    temp = temp+Money[2]*16;
    temp = temp+Money[3];
    temp = temp+Money[5]*0.1;

    QString mon = QString("%1").arg(temp);//将temp转化字符串
    ui->Log_MonylineEdit->insert(mon);

    QString Item = str.mid(14,2);

    if(Item == "10")//消费
	Update_Database(RFID,mon);
    else if(Item == "20")//充值
	Update_Database(RFID,mon);
}

QString strs;//储存串口读到的数据

void Widget::ReadCom()//读取数据
{
    //读取串口缓冲区的所有数据给临时变量temp
    QByteArray temp = MyCom->readAll();

    QDataStream out(&temp,QIODevice::ReadWrite);    //将字节数组读入

    while(!out.atEnd())
    {
	qint8 outChar = 0;
	out>>outChar;   //每字节填充一次，直到结束
	//十六进制的转换
	strs = strs + QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
    }

    //由于串口不是一次性将所有数据读取出来，所以要用 strs 字符串储存起来在进行处理
    if(strs.length() >= 15)//数据接收完成
    {
	//ui->textEdit->insertPlainText(strs);
	Del_String(strs);//处理字符串
	strs.clear();//清空字符串
    }
}



void Widget::on_RFID_ClosepushButton_clicked()
{
    MyCom->close();
    delete MyCom;

    ui->RFID_ClosepushButton->setVisible(false);
    ui->RFID_OpenpushButton->setVisible(true);
}


#if 0

//以16进制发送命令
void Widget::on_RFID_CmdpushButton_clicked()
{
    QString str = ui->RFID_lineEdit->text();

    ui->RFID_textEdit->insertPlainText(str);

    QByteArray senddata;

    StringToHex(str,senddata);

    MyCom->write(senddata);
}



void Widget::StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
	//char lstr,
	hstr=str[i].toLatin1();//有问题
	if(hstr == ' ')
	{
	    i++;
	    continue;
	}
	i++;
	if(i >= len)
	    break;
	lstr = str[i].toLatin1();
	hexdata = ConvertHexChar(hstr);
	lowhexdata = ConvertHexChar(lstr);
	if((hexdata == 16) || (lowhexdata == 16))
	    break;
	else
	    hexdata = hexdata*16+lowhexdata;
	i++;
	senddata[hexdatalen] = (char)hexdata;
	hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char Widget::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
	    return ch-0x30;
	else if((ch >= 'A') && (ch <= 'F'))
	    return ch-'A'+10;
	else if((ch >= 'a') && (ch <= 'f'))
	    return ch-'a'+10;
	else return (-1);
}
#endif
