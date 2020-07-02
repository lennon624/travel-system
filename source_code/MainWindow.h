#pragma once

#include <QtWidgets/QMainWindow>
#include <QListWidget>
#include <vector>
#include <QAction>
#include <QProgressBar>
#include "Transport.h"
#include "TransportFrame.h"
#include "ui_MainWindow.h"
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
private:
    Ui::MainWindowClass ui;

    /*״̬���ı�ǩ*/
    QLabel* currSrcCity;
    QLabel* currDestCity;
    QProgressBar* currProgress;
    QLabel* currTime;
    QLabel* currDay;
    QLabel* currUserStatus;

    User* user;
    TransSystem* sys;

    int hTimerId; /*ʱ��timer��id*/
    const int MS_PER_H = 200;/*ÿСʱ���ٺ���*/
    vector<Transport> planCache;
    //int gTime;  /*ʱ��*/
    //int gDay;   /*����*/

	void SetCityList(QComboBox* comboBox, const vector<City>& listCity);
    void SetTransList(QListWidget* listWidget, const vector<Transport>& listTrans);
    void UpdateStatusBar();
    void UpdatePageTravel();
};
