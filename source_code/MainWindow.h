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
    /*������ui*/
    Ui::MainWindowClass ui;
    /*���ô���ui*/
    Ui::SettingsWindow ui_settings;
    QWidget* settingsWindow;

    /*״̬���ı�ǩ*/
    QLabel* currSrcCity;
    QLabel* currDestCity;
    QProgressBar* currProgress;
    QLabel* currTime;
    QLabel* currDay;
    QLabel* currUserStatus;

    /*��ͼҳ*/
    MapCanvas* mapCanvas;

    User* user;
    TransSystem* sys;

    int hTimerId; /*ʱ��timer��id*/
    const int MS_PER_H = 2000;/*ÿСʱ���ٺ���*/
    vector<Transport> planCache;
    //int gTime;  /*ʱ��*/
    //int gDay;   /*����*/

	void SetCityList(QComboBox* comboBox, const vector<City>& listCity);
	void SetTransList(QListWidget* listWidget, bool viewTrueDay, const vector<Transport>& listTrans);
    void UpdateStatusBar();
    void UpdatePageTravel();
    void ShowBestPlan();
    void OnSettingChanged(int state); /*�������е����ñ仯*/

};


