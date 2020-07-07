#include "MapCanvas.h"

MapCanvas::MapCanvas(QWidget *parent)
	: QWidget(parent),
	imgMap(new QPixmap(":/img/map.png"))
{
	ui.setupUi(this);
	/*˼·*/
	/*
		��ʼ��pixmap��Ҫ�����ųɴ��ڿ�ʼ��״̬�������浱ǰ�ĳߴ硣
		ʹ������ó�ʼ���ڴ�Сʱ����������(�����ͼƬ�����Ͻ�)
		дһ��CityLocs���飬�������ֶ�¼��ȥ��
		����һ��ȫ�ֱ���double ratio��
		ÿ��ˢ����Ļ�󣬼��㵱ǰ����ڳ�ʼ��ratio������ʼ��x�����½�ȥ��
		
		ÿ�λ�ͼ��ʱ�򣬼����������Ĺ�ʽΪ:
			x[i] = mapX + CityLocs[i].x*ratio
			y[i] = CityLocs[i].y * ratio
		��ʼΪ753,753;
		
		487 , 125	����
		198 , 415	�ɶ�
		314 , 305	����
		385 , 203	̫ԭ
		615 , 73	����
		424 , 523	��ɳ
		454 , 428	�人
		537 , 379	�Ϸ�
		564 , 234	����
		462 , 656	����
		606 , 546	����
		634 , 450	����

	*/
}

MapCanvas::~MapCanvas()
{
}

void MapCanvas::paintEvent(QPaintEvent* ev)
{
	QPainter p;
	p.begin(this);
	QPixmap mapScaled = imgMap->scaledToHeight(this->height(), Qt::SmoothTransformation);/*���߶ȵȱ�������*/
	mapX = this->x() + this->width() / 2 - mapScaled.width() / 2;	/*���µ�ͼ�����Ͻ�����*/
	ratio = mapScaled.height() * 1.0 / originH;						/*���µ�ǰ�����ű���*/
	p.drawPixmap(mapX, 0, mapScaled);
	p.end();

}
void MapCanvas::mousePressEvent(QMouseEvent* ev)
{
	qDebug() << ev->x()-mapX << "," << ev->y();
}
/*ˢ��,��ͼ�¼�*/
