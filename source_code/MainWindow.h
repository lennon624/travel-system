#pragma once

#include <QtWidgets/QMainWindow>
#include <QListWidget>
#include <vector>
#include <QAction>
#include <QProgressBar>
#include <QMessageBox>
#include "Transport.h"
#include "TransportFrame.h"
#include "ui_MainWindow.h"
#include "ui_SettingsWindow.h"
#include "User.h"
#include "QListWidget"

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

    User* user;
    TransSystem* sys;

    int hTimerId; /*时钟timer的id*/
    const int MS_PER_H = 2000;/*每小时多少毫秒*/
    vector<Transport> planCache;
    //int gTime;  /*时间*/
    //int gDay;   /*日期*/

	void SetCityList(QComboBox* comboBox, const vector<City>& listCity);
	void SetTransList(QListWidget* listWidget, bool viewTrueDay, const vector<Transport>& listTrans);
    void UpdateStatusBar();
    void UpdatePageTravel();
    void ShowBestPlan();
    void OnSettingChanged(int state); /*处理所有的设置变化*/

};


