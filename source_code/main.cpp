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
#include <QFont>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*设置样式表*/
    QFile qss(":/Diffnes.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    qss.close();

    /*设置字体*/
    int id = QFontDatabase::addApplicationFont(":/MONACO.TTF");
    a.setFont(QFont(QFontDatabase::applicationFontFamilies(id).at(0)));

    /*创建窗口*/
    MainWindow w;
    w.show();

    return a.exec();
}
