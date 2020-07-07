/*******************************************
 * Project:         travel-system
 * Discription:     ��������·�߹滮ϵͳ
 * Author:          �����
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
    /*���ñ���*/
    QApplication a(argc, argv);

    /*������ʽ��*/
    QFile qss(":/Diffnes.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    qss.close();

    ///*��������*/
    //int id = QFontDatabase::addApplicationFont(":/MONACO.TTF");
    //int id = QFontDatabase::addApplicationFont(":/msyh.ttc");
    //a.setFont(QFont(QFontDatabase::applicationFontFamilies(id).at(0)));

    a.setFont(QFont("Microsoft YaHei UI", 14));
    /*��������*/
    MainWindow w;
    w.show();

    return a.exec();
}
