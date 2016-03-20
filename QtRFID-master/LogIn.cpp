#include "widget.h"
#include "ui_widget.h"

//以下函数都是操作注册界面的


//注册用户
void Widget::on_LogIn_OKpushButton_clicked()
{
    QSqlQuery query;

    QString Name = ui->Log_NamelineEdit->text();
    QString SFID = ui->Log_SFIDlineEdit->text();
    QString Tel = ui->Log_NumlineEdit->text();
    QString Room = ui->Log_RoomlineEdit->text();
    QString Money = ui->Log_MonylineEdit->text();
    QString RFID = ui->Log_RFIDlineEdit->text();

    if(RFID == NULL||Money == NULL||Room == NULL||Tel == NULL||SFID == NULL||Name==NULL)
	QMessageBox::warning(this,tr("警告"),tr("注册信息不能为空！"),QMessageBox::Ok);
    else
    {
	QString Cmds = "INSERT INTO `gravida`(`name`,`idcard`,`tel`,`roomID`,`money`,`rfid`,`serverID`,`status`,`accesstime`,`updatetime`)"
		+ QString("VALUES ('%1', '%2', '%3', '%4','%5', '%6', '00010','0','20150812','20150913')").arg(Name,SFID,Tel,Room,Money,RFID);

	//ui->textEdit->insertPlainText(Cmds);

	if(query.exec(Cmds))//发送命令到数据库
	    QMessageBox::information(this,tr("注册"),tr("注册成功！"),QMessageBox::Ok);
	else
	    QMessageBox::warning(this,tr("警告"),tr("注册失败！"),QMessageBox::Ok);
    }

}

//注销用户
void Widget::on_Log_LogOutpushButton_clicked()
{\
    QMessageBox::StandardButton replay;

    QString RFID = ui->Log_RFIDlineEdit->text();

    if(RFID == NULL)
	QMessageBox::warning(this,tr("警告"),tr("请输入卡号！"),QMessageBox::Ok);
    else
    {
	replay = QMessageBox::question(this,tr("确认"), tr("确认要注销此用户？"),
			      QMessageBox::Yes|QMessageBox::No);

	if(replay == QMessageBox::Yes)
	{
	    QSqlQuery query;

	    QString Cmds = QString("DELETE FROM `gravidacare`.`gravida`WHERE`gravida`.`rfid`='%1' LIMIT 1").arg(RFID);
	    if(query.exec(Cmds))
		QMessageBox::information(this,tr("注销"),tr("注销成功！"),QMessageBox::Ok);
	    else
		QMessageBox::warning(this,tr("警告"),tr("注销失败！"),QMessageBox::Ok);
	}
    }
}

