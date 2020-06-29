#pragma once

#include <QtWidgets/QMainWindow>
#include <QListWidget>
#include <vector>
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
    User* user;
    TransSystem* sys;
    int gTime;

	void SetCityList(QListWidget* listWidget, const vector<City>& listCity);
    void SetTransList(QListWidget* listWidget, const vector<Transport>& listTrans);

};
