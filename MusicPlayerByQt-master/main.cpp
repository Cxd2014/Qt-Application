#include <QtGui/QApplication>
#include "widget.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//ʹ��������ʾ
    Widget w;
    w.show();
    
    return a.exec();
}
