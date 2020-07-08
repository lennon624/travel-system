#pragma once

// 画图类，重写paintEvent执行绘图事件
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <vector>
#include "Transport.h"
#include "ui_MapCanvas.h"
using namespace std;

class MapCanvas : public QWidget
{
	Q_OBJECT

public:
	MapCanvas(QWidget *parent = Q_NULLPTR);
	~MapCanvas();

	/*API: 添加plan,并立刻在图中显示出来*/
	void AddPlan(const vector<Transport>transList) { m_transList = transList; }

	/*API: 返回是否空*/
	bool PlanEmpty() { return m_transList.empty(); }

	/*API: 更新并显示当前状态*/
	void UpdateStatus(int transIndex, int progress);

	/*API: plan已经完成了,清空当前的plan*/
	void ClearPlan() { m_transList.clear(); }

protected:
	void paintEvent(QPaintEvent* ev)override;

	void mousePressEvent(QMouseEvent* ev)override;


private:
	Ui::MapCanvas ui;

	/*添加地图背景*/
	QPixmap* imgMap;
	QPixmap* imgBus;	/*汽车贴图*/
	QPixmap* imgTrain;	/*火车贴图*/
	QPixmap* imgPlane;	/*飞机贴图*/

	int mapX;				/*当前地图的左上角坐标*/
	const int originW = 753;/*初始图片的尺寸*/
	const int originH = 753;
	double ratio;

	/*所有城市的坐标数组*/	
	vector<QPoint> m_cityLoc;
	vector<Transport> m_transList;
	int m_currIndex;	/*当前transport的序号*/
	int m_currProgress;	/*当前transport的进度*/

	
	/*求箭头坐标的代码*/
	void calcVertexes(double start_x, double start_y, double end_x, double end_y, double& x1, double& y1, double& x2, double& y2)
	{
		double arrow_lenght_ = 25;//箭头长度，一般固定
		double arrow_degrees_ = 0.5;//箭头角度，一般固定
		double angle = atan2(end_y - start_y, end_x - start_x) + 3.1415926;
		x1 = end_x + arrow_lenght_ * cos(angle - arrow_degrees_);//求得箭头点1坐标
		y1 = end_y + arrow_lenght_ * sin(angle - arrow_degrees_);
		x2 = end_x + arrow_lenght_ * cos(angle + arrow_degrees_);//求得箭头点2坐标
		y2 = end_y + arrow_lenght_ * sin(angle + arrow_degrees_);
	}
};
