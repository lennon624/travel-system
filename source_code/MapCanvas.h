#pragma once

// ��ͼ�࣬��дpaintEventִ�л�ͼ�¼�
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

	/*��ӵ�ͼ����*/
	QPixmap* imgMap;
	int mapX;
	const int originW = 753;/*��ʼͼƬ�ĳߴ�*/
	const int originH = 753;
	double ratio;
};
