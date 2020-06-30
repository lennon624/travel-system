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

private:
    Ui::MainWindowClass ui;

    /*×´Ì¬À¸µÄ±êÇ©*/
    QLabel* currSrcCity;
    QLabel* currDestCity;
    QProgressBar* currProgress;

    User* user;
    TransSystem* sys;
    int gTime;

	void SetCityList(QComboBox* comboBox, const vector<City>& listCity);
    void SetTransList(QListWidget* listWidget, const vector<Transport>& listTrans);
    void UpdateStatusBar();
};
