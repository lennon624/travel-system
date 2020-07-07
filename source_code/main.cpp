/*******************************************
 * Project:         travel-system
 * Discription:     智能旅行路线规划系统
 * Author:          邓杨燊
 * Creation Date:   2020/5/26
 *******************************************/
#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <QPushbutton>
#include <QFile>
//#include <QFont>
//#include <QFontDatabase>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    /*设置编码*/
    QApplication a(argc, argv);

    /*设置样式表*/
    QFile qss(":/Diffnes.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    qss.close();

    ///*设置字体*/
    //int id = QFontDatabase::addApplicationFont(":/MONACO.TTF");
    //int id = QFontDatabase::addApplicationFont(":/msyh.ttc");
    //a.setFont(QFont(QFontDatabase::applicationFontFamilies(id).at(0)));

    a.setFont(QFont("Microsoft YaHei UI", 14));
    /*创建窗口*/
    MainWindow w;
    w.show();

    return a.exec();
}
