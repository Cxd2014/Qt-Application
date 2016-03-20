#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "qextserialport.h"
#include <QUdpSocket>
#include <QNetworkAccessManager>

#define PEOPLENUM 12 //需要监视的总人数
#define PORT 6666          //端口号

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT
	
public:
	explicit Widget(QWidget *parent = 0);
	~Widget();
	Ui::Widget *ui;
	
private slots:
	void ReadCom(); //接受串口数据
	void CheckOnLine(); //检查节点是否在线
	
	void on_toolButton_1_clicked();
	
	void on_toolButton_2_clicked();
	
	void on_toolButton_3_clicked();
	
	void on_toolButton_4_clicked();
	
	void on_toolButton_5_clicked();
	
	void on_toolButton_6_clicked();
	
	void on_toolButton_7_clicked();
	
	void on_toolButton_8_clicked();
	
	void on_toolButton_9_clicked();
	
	void on_toolButton_10_clicked();
	
	void on_toolButton_11_clicked();
	
	void on_toolButton_12_clicked();
	
	void on_toolButton_13_clicked();
	
	void on_Connect_toolButton_clicked();
	
	void on_Disconnect_toolButton_clicked();
	
	void on_Open_toolButton_clicked();
	
	void on_Close_toolButton_clicked();
	
	void on_Clear_toolButton_clicked();
	
	
	
signals:
	void SignalTimer(); // 每隔 CheckTimer 个时间发送一个信号检查节点状态
	
private:
	
	QextSerialPort *myCom; //定义一个串口  全局变量
	
	QString ServerUrl; //记录发送到服务器的 HTTP 链接
	QString ipAdress; //记录IP地址
	
	/*
	 * 检查节点在线原理是这样的，节点每发送一个在线信息 Flag 数组相应的加 1 ，
	 * 然后每隔 CheckTimer 个时间就比较一次 Flag 数组与 CheckFlag[i][0] 是否相等
	 * 若相等则说明 对应节点掉线，若不相等则将 Flag 的数据更新到 CheckFlag[i][0] 中
	 * CheckFlag[i][1] 记录节点上一次检查时的状态，与这一次状态比较，若一样则不需要发送信息给服务器
	 */
	
	int Flag[PEOPLENUM]; //记录在线状态
	void sortMessage(QByteArray Data); //处理在线信息
	
	QTimer *CheckTimer; 
	int CheckFlag[PEOPLENUM][2];
	
	void ChangeNodeStatus(int i,int j); //改变节点状态，并发送HTTP请求

	QNetworkAccessManager *manager; //定义一个发送HTTP的对象
};

#endif // WIDGET_H
