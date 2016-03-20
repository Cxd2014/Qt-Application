#include "widget.h"
#include "ui_widget.h"

QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");//加载QMYSQL驱动

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);


/************************操作数据库界面***************************/
    //设置默认值
    ui->IpAdresslineEdit->setText("192.168.1.123");
    ui->UserNamelineEdit->setText("CXD");
    ui->PasswordlineEdit->setText("921120");
    ui->CmdpushButton->setDisabled(true);
    ui->SqlClosepushButton->setVisible(false);//隐藏数据库关闭按钮
    ui->textEdit->setReadOnly(true);//使TextEdit只能读不能写

    QPalette  Red;         //设置按钮上面的字体颜色
    Red.setColor(QPalette::ButtonText, QColor(255,0,0));
    ui->SqlClosepushButton->setPalette(Red);

/************************操作注册界面***************************/
    ui->LogIn_OKpushButton->setDisabled(true);
    ui->Log_LogOutpushButton->setDisabled(true);

/************************操作RFID界面***************************/
    //ui->RFID_ClosepushButton->setDisabled(true);
    ui->RFID_ClosepushButton->setVisible(false);
    ui->RFID_ClosepushButton->setPalette(Red);
}

Widget::~Widget()
{
    delete ui;
}


//以下函数都是操作数据库界面的,打开数据库
void Widget::on_SqlConnectButton_clicked()
{
    //QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");//加载QMYSQL驱动

    QString IP = ui->IpAdresslineEdit->text();
    db.setHostName(IP);//设置你远程服务器的IP

    db.setPort(3306);//设置端口

    db.setDatabaseName("gravidacare");//数据库名


    QString UserName = ui->UserNamelineEdit->text();
    db.setUserName(UserName);//数据库用户名和密码
    QString Password = ui->PasswordlineEdit->text();
    db.setPassword(Password);

    if (db.open())
    {
	ui->textEdit->insertPlainText("Succeed Connect the SQL!\n");  
	ui->CmdpushButton->setEnabled(true);
	ui->SqlConnectButton->setVisible(false);//隐藏打开按钮
	ui->SqlClosepushButton->setVisible(true);//显示关闭按钮

	ui->LogIn_OKpushButton->setEnabled(true);
	ui->Log_LogOutpushButton->setEnabled(true);
	//CXD = 1;//数据库打开
    }
    else
	ui->textEdit->insertPlainText("Failed Connect the SQL!\n");
}

//查询数据库
void Widget::on_CmdpushButton_clicked()
{
    QSqlQuery query;

    QString Note[6] = {"姓  名 ： ","身份证号：","电话号码：","房间号 ： ","余  额 ： ","RFID卡号："};

    QString Cmd1 = ui->CmdlineEdit->text();
    QString Cmds;

    char c_bt1 = ui->SelectcomboBox->currentIndex();

    switch(c_bt1)
    {
	case 0: Cmds = "select * from gravida WHERE name = ";break;//数据库以名字查询客户信息
	case 1: Cmds = "select * from gravida WHERE roomID =";break;//数据库以房间号查询客户信息
	case 2: Cmds = "select * from gravida";break;//查询所有客户信息
	default : break;
    }


    if(Cmd1 == NULL && c_bt1 != 2)
	QMessageBox::warning(this,tr("警告"),tr("请输入要查询的用户信息！"),QMessageBox::Ok);
    else
    {
	if(c_bt1!=2)
	    Cmds = Cmds + "'" + Cmd1 + "'";

	ui->textEdit->insertPlainText(Cmds + "\n\n");//将命令显示出来

	query.exec(Cmds);//发送命令到数据库

	while(query.next())//显示所有的数据对象
	{
	    for(int i = 1;i<7;i++)//每个数据对象有6个属性
	    {
		QString Item = query.value(i).toString().trimmed();
		ui->textEdit->insertPlainText(Note[i-1]+Item + "\n");
	    }
	    ui->textEdit->insertPlainText("\n");
	}

    }

}

void Widget::on_ClearpushButton_clicked()
{
    ui->textEdit->clear();
}


//关闭数据库
void Widget::on_SqlClosepushButton_clicked()
{
    db.close();//关闭数据库

    ui->LogIn_OKpushButton->setDisabled(true);
    ui->Log_LogOutpushButton->setDisabled(true);
    ui->textEdit->insertPlainText("Closed the SQL!\n");
    ui->SqlConnectButton->setVisible(true);
    ui->SqlClosepushButton->setVisible(false);
    ui->CmdpushButton->setDisabled(true);

}

//每次消费和充值都要更新数据库
void Widget::Update_Database(QString str1,QString str2)
{
    QSqlQuery query;
    QString Cmds;

    Cmds = QString("select * from gravida WHERE rfid = '%1'").arg(str1);//查询此卡是否注册

    if(!query.exec(Cmds))
	QMessageBox::warning(this,tr("失败"),tr("数据库未连接！"),QMessageBox::Ok);
    else
    {
	if(!query.next())
	    QMessageBox::warning(this,tr("更新失败"),tr("此卡未注册！"),QMessageBox::Ok);
	else //更新数据
	{

	    ui->Log_NamelineEdit->insert(query.value(1).toString().trimmed());
	    ui->Log_SFIDlineEdit->insert(query.value(2).toString().trimmed());
	    ui->Log_NumlineEdit->insert(query.value(3).toString().trimmed());
	    ui->Log_RoomlineEdit->insert(query.value(4).toString().trimmed());

	    Cmds = "UPDATE `gravidacare`.`gravida` SET " +
		   QString("`money` = '%1' WHERE `gravida`.`rfid` = '%2' LIMIT 1;").arg(str2,str1);

	    //ui->textEdit->insertPlainText(Cmds);
	    if(!query.exec(Cmds))//发送命令到数据库
		QMessageBox::warning(this,tr("失败"),tr("更新失败！"),QMessageBox::Ok);
	}
    }
}






