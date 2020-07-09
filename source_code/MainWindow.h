#pragma once

#include <QtWidgets/QMainWindow>
#include <QHboxLayout>
#include <QVboxLayout>
#include <QListWidget>
#include <vector>
#include <QAction>
#include <QProgressBar>
#include <QMessageBox>
#include <QPainter>
#include <QListWidget>
#include <fstream>
#include "ui_MainWindow.h"
#include "ui_SettingsWindow.h"
#include "Transport.h"
#include "TransportFrame.h"
#include "User.h"
#include "MapCanvas.h"

using namespace std;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

protected:
	void timerEvent(QTimerEvent* ev)override;
    void closeEvent(QCloseEvent* ev)override;

private:
    /*主窗口ui*/
    Ui::MainWindowClass ui;
    /*设置窗口ui*/
    Ui::SettingsWindow ui_settings;
    QWidget* settingsWindow;

    /*状态栏的标签*/
    QLabel* currSrcCity;
    QLabel* currDestCity;
    QProgressBar* currProgress;
    QLabel* currTime;
    QLabel* currDay;
    QLabel* currUserStatus;

    /*地图页*/
    MapCanvas* mapCanvas;

    User* user;
    TransSystem* sys;

    int hTimerId; /*时钟timer的id*/
    int MS_PER_H = 2000;/*每小时多少毫秒*/
    vector<Transport> planCache;
    //int gTime;  /*时间*/
    //int gDay;   /*日期*/

	void SetCityList(QComboBox* comboBox, const vector<City>& listCity);
	void SetTransList(QListWidget* listWidget, bool viewTrueDay, const vector<Transport>& listTrans);
    void UpdateStatusBar();
    void UpdatePageTravel();
    void ShowBestPlan();
    void OnSettingChanged(int state); /*处理所有的设置变化*/

    void logCout(const QString& str) {     /*打印函数*/
        static int i = 0;
        QFile file("travel.log");
        if(i==0){ 
            ++i;
            file.open(QFile::Truncate | QFile::WriteOnly);
        } else {
			file.open(QFile::Append | QFile::WriteOnly);
        }
        QTextStream s(&file);
        s << QString(u8"第 %1 天 %2:00  %3")
            .arg(sys->GetDay())
            .arg(sys->GetTime())
            .arg(str)
            << endl;
            
            //<< u8"第 " << sys->GetDay() << u8" 天  " << sys->GetTime()
            //<< u8":00   "<< str << endl;   /*打印*/
    }
    const QString TransListToQString(const vector<Transport>translist) {
        /*将计划变成字符串*/
        if (translist.empty()) {
            return u8"空";
        } else {
            /*写起始城市*/
            QString str(QString::fromStdString(
                sys->GetCityList()
                .at(translist
                    .at(0).m_srcIndex).m_name));
            /*写后续城市*/
            for (auto i:translist)
            {
                str.push_back(u8"->" + QString::fromStdString(
					sys->GetCityList().at(i.m_destIndex).m_name)
                );
            }
            return str;
        }
    }
};


