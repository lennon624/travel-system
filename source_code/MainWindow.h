#pragma once

#include <QtWidgets/QMainWindow>
#include <QListWidget>
#include "TransportFrame.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::MainWindowClass ui;
};
