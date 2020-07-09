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
    int MS_PER_H = 2000;/*ÿСʱ���ٺ���*/
    vector<Transport> planCache;
    //int gTime;  /*ʱ��*/
    //int gDay;   /*����*/

	void SetCityList(QComboBox* comboBox, const vector<City>& listCity);
	void SetTransList(QListWidget* listWidget, bool viewTrueDay, const vector<Transport>& listTrans);
    void UpdateStatusBar();
    void UpdatePageTravel();
    void ShowBestPlan();
    void OnSettingChanged(int state); /*�������е����ñ仯*/

    void logCout(const QString& str) {     /*��ӡ����*/
        static int i = 0;
        QFile file("travel.log");
        if(i==0){ 
            ++i;
            file.open(QFile::Truncate | QFile::WriteOnly);
        } else {
			file.open(QFile::Append | QFile::WriteOnly);
        }
        QTextStream s(&file);
        s << QString(u8"�� %1 �� %2:00  %3")
            .arg(sys->GetDay())
            .arg(sys->GetTime())
            .arg(str)
            << endl;
            
            //<< u8"�� " << sys->GetDay() << u8" ��  " << sys->GetTime()
            //<< u8":00   "<< str << endl;   /*��ӡ*/
    }
    const QString TransListToQString(const vector<Transport>translist) {
        /*���ƻ�����ַ���*/
        if (translist.empty()) {
            return u8"��";
        } else {
            /*д��ʼ����*/
            QString str(QString::fromStdString(
                sys->GetCityList()
                .at(translist
                    .at(0).m_srcIndex).m_name));
            /*д��������*/
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


