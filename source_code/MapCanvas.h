#pragma once

// 画图类，重写paintEvent执行绘图事件
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include "ui_MapCanvas.h"

class MapCanvas : public QWidget
{
	Q_OBJECT

public:
	MapCanvas(QWidget *parent = Q_NULLPTR);
	~MapCanvas();

protected:
	void paintEvent(QPaintEvent* ev)override;

	void mousePressEvent(QMouseEvent* ev)override;

private:
	Ui::MapCanvas ui;

	/*添加地图背景*/
	QPixmap* imgMap;
	int mapX;
	const int originW = 753;/*初始图片的尺寸*/
	const int originH = 753;
	double ratio;
};
