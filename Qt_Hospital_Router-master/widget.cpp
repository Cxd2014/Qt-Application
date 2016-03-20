#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

const QString OffLine = "background-color: rgb(0,0,0);color:rgb(255,255,255);";
const QString OnLine = "background-color: rgb(255,255,255);color:rgb(0,0,0);";

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);
	ui->Disconnect_toolButton->setDisabled(true);
	ui->Close_toolButton->setDisabled(true);
	ui->IP_lineEdit->setText("192.168.0.101:80");
	
	for(int i = 0;i<PEOPLENUM;i++){ //初始化
		Flag[i] = 0;
		CheckFlag[i][0] = 0;
		CheckFlag[i][1] = 0;
	}
	
	ui->Patient_toolButton_1->setStyleSheet(OffLine);
	ui->Patient_toolButton_2->setStyleSheet(OffLine);
	ui->Patient_toolButton_3->setStyleSheet(OffLine);
	ui->Patient_toolButton_4->setStyleSheet(OffLine);
	ui->Patient_toolButton_5->setStyleSheet(OffLine);
	ui->Patient_toolButton_6->setStyleSheet(OffLine);
	ui->Patient_toolButton_7->setStyleSheet(OffLine);
	ui->Patient_toolButton_8->setStyleSheet(OffLine);
	ui->Patient_toolButton_9->setStyleSheet(OffLine);
	ui->Patient_toolButton_10->setStyleSheet(OffLine);
	ui->Patient_toolButton_11->setStyleSheet(OffLine);
	ui->Patient_toolButton_12->setStyleSheet(OffLine);
	
	CheckTimer = new QTimer;
	CheckTimer->start(30000); // 每隔 18 秒检查一次节点
	connect(CheckTimer,SIGNAL(timeout()),this,SLOT(CheckOnLine()));
	
	//sender =new QUdpSocket; //建立一个 UdpSocket 连接
	
	manager = new QNetworkAccessManager(this); //发送HTTP请求
	
}

Widget::~Widget()
{
	delete ui;
}

QByteArray ComData;
/*读取串口数据
* 由于串口每次读取的数据不是整条命令，所以要确定收到一整条命令后在
* 进行处理，一条完整的指令的字节数为 9 所以当字节数大于等于9时认为是
* 一条完整指令。(但是这样偶尔会丢失命令，暂时还没有想到解决办法)
*/
void Widget::ReadCom()
{
	ComData = ComData + myCom->readAll();
	
	int i = ComData.length();
	if(i>=9){//表示是一条完整指令	
		sortMessage(ComData);
		ComData.clear();
	}
}

//ONN00001#     护士节点代号
//ONP00002#     病人节点代号
//ONR00001#     路由器节点代号
QString LastCES;
void Widget::sortMessage(QByteArray Data)
{
	QString S = QString::fromAscii(Data.data());
	ui->textEdit->insertPlainText(S);
	
	if(!(ui->IP_lineEdit->isEnabled())){ //已连接服务器
		QString QItem = QString(Data.mid(0,3));
		QString Qnum = QString(Data.mid(4,4));
			
		if(QItem == "ONP"){//在线消息	
			int Inum = Qnum.toInt();
			Flag[Inum] =Flag[Inum] +1 ;
			return;
		}
		
		if(QItem == "CAL"){//呼叫信息
			manager->get(QNetworkRequest(QUrl(ServerUrl + "call?id=0" + Qnum)));
			return;
		}
		
		if(QItem == "CON"){//确认呼叫
			manager->get(QNetworkRequest(QUrl(ServerUrl + "confirm?id=0" + Qnum)));
			return;
		}
		
		if(QItem == "CAN"){//取消呼叫
			manager->get(QNetworkRequest(QUrl(ServerUrl + "cancel?id=0" + Qnum)));
			return;
		}
		
		if(QItem == "CES"){//温湿度
			if(LastCES != Qnum){
				LastCES = Qnum;
				manager->get(QNetworkRequest(QUrl(ServerUrl + "data?te=0" + LastCES)));
			}
		}
	}	
}

void Widget::CheckOnLine() //检查节点是否在线
{
	int i;
	const QChar fill = '0';
	for(i=0;i<PEOPLENUM;i++){
		if(CheckFlag[i][0] == Flag[i] ){ //掉线
			if(CheckFlag[i][1] == 1){ //上一次状态在线，所以要改变状态为掉线
				QString ID = QString("%1").arg(i,5,10,fill);
				manager->get(QNetworkRequest(QUrl(ServerUrl+"offline?id="+ID))); //向服务器发送掉线信息
				ChangeNodeStatus(i,0);
				CheckFlag[i][1] = 0;
			}
		}else{ // 在线
			CheckFlag[i][0] = Flag[i];
			if(CheckFlag[i][1] == 0) {//上一次状态掉线，所以要改变状态为在线
				QString ID = QString("%1").arg(i,5,10,fill);
				manager->get(QNetworkRequest(QUrl(ServerUrl+"online?id="+ID)));//向服务器发送掉线信息
				ChangeNodeStatus(i,1);
				CheckFlag[i][1] = 1;
			}
		}
	}
}

void Widget::ChangeNodeStatus(int i,int j)
{
	switch(i)
	{
		case 1:
			if(j == 0)
				ui->Patient_toolButton_1->setStyleSheet(OffLine);
			else
				ui->Patient_toolButton_1->setStyleSheet(OnLine);
			break;
			
		case 2:
			if(j == 0)
				ui->Patient_toolButton_2->setStyleSheet(OffLine);
			else
				ui->Patient_toolButton_2->setStyleSheet(OnLine);
			break;
			
		case 3:
			if(j == 0)
				ui->Patient_toolButton_3->setStyleSheet(OffLine);
			else
				ui->Patient_toolButton_3->setStyleSheet(OnLine);
			break;
		case 4:
			if(j == 0)
				ui->Patient_toolButton_4->setStyleSheet(OffLine);
			else
				ui->Patient_toolButton_4->setStyleSheet(OnLine);
			break;
		case 5:
			if(j == 0)
				ui->Patient_toolButton_5->setStyleSheet(OffLine);
			else
				ui->Patient_toolButton_5->setStyleSheet(OnLine);
			break;
		case 6:
			if(j == 0)
				ui->Patient_toolButton_6->setStyleSheet(OffLine);
				else
				ui->Patient_toolButton_6->setStyleSheet(OnLine);
			break;
		case 7:
			if(j == 0)
				ui->Patient_toolButton_7->setStyleSheet(OffLine);
			else
				ui->Patient_toolButton_7->setStyleSheet(OnLine);
			break;
		case 8:
			if(j == 0)
				ui->Patient_toolButton_8->setStyleSheet(OffLine);
			else
				ui->Patient_toolButton_8->setStyleSheet(OnLine);
			break;
		case 9:
			if(j == 0)
				ui->Patient_toolButton_9->setStyleSheet(OffLine);
			else
				ui->Patient_toolButton_9->setStyleSheet(OnLine);
			break;
		case 10:
			if(j == 0)
				ui->Patient_toolButton_10->setStyleSheet(OffLine);
			else
				ui->Patient_toolButton_10->setStyleSheet(OnLine);
			break;
		case 11:
			if(j == 0)
				ui->Patient_toolButton_11->setStyleSheet(OffLine);
			else
				ui->Patient_toolButton_11->setStyleSheet(OnLine);
			break;
		case 12:
			if(j == 0)
				ui->Patient_toolButton_12->setStyleSheet(OffLine);
			else
				ui->Patient_toolButton_12->setStyleSheet(OnLine);
			break;
	}
}

