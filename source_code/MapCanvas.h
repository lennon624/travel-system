#pragma once

// ��ͼ�࣬��дpaintEventִ�л�ͼ�¼�
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

	/*API: ���plan,��������ͼ����ʾ����*/
	void AddPlan(const vector<Transport>transList) { m_transList = transList; }

	/*API: �����Ƿ��*/
	bool PlanEmpty() { return m_transList.empty(); }

	/*API: ���²���ʾ��ǰ״̬*/
	void UpdateStatus(int transIndex, int progress);

	/*API: plan�Ѿ������,��յ�ǰ��plan*/
	void ClearPlan() { m_transList.clear(); }

protected:
	void paintEvent(QPaintEvent* ev)override;

	void mousePressEvent(QMouseEvent* ev)override;


private:
	Ui::MapCanvas ui;

	/*��ӵ�ͼ����*/
	QPixmap* imgMap;
	QPixmap* imgBus;	/*������ͼ*/
	QPixmap* imgTrain;	/*����ͼ*/
	QPixmap* imgPlane;	/*�ɻ���ͼ*/

	int mapX;				/*��ǰ��ͼ�����Ͻ�����*/
	const int originW = 753;/*��ʼͼƬ�ĳߴ�*/
	const int originH = 753;
	double ratio;

	/*���г��е���������*/	
	vector<QPoint> m_cityLoc;
	vector<Transport> m_transList;
	int m_currIndex;	/*��ǰtransport�����*/
	int m_currProgress;	/*��ǰtransport�Ľ���*/

	
	/*���ͷ����Ĵ���*/
	void calcVertexes(double start_x, double start_y, double end_x, double end_y, double& x1, double& y1, double& x2, double& y2)
	{
		double arrow_lenght_ = 25;//��ͷ���ȣ�һ��̶�
		double arrow_degrees_ = 0.5;//��ͷ�Ƕȣ�һ��̶�
		double angle = atan2(end_y - start_y, end_x - start_x) + 3.1415926;
		x1 = end_x + arrow_lenght_ * cos(angle - arrow_degrees_);//��ü�ͷ��1����
		y1 = end_y + arrow_lenght_ * sin(angle - arrow_degrees_);
		x2 = end_x + arrow_lenght_ * cos(angle + arrow_degrees_);//��ü�ͷ��2����
		y2 = end_y + arrow_lenght_ * sin(angle + arrow_degrees_);
	}
};
