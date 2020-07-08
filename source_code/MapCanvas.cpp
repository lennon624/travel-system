#include "MapCanvas.h"

MapCanvas::MapCanvas(QWidget *parent)
	: QWidget(parent),
	imgMap(new QPixmap(":/img/map.png")),
	imgBus(new QPixmap(":/img/bus.png")),
	imgTrain(new QPixmap(":/img/train.png")),
	imgPlane(new QPixmap(":/img/airplane.png")),
	m_transList({})
{
	ui.setupUi(this);

	/*����ͼ��*/
	*imgBus = imgBus->scaledToHeight(60, Qt::SmoothTransformation);
	*imgTrain = imgTrain->scaledToHeight(60, Qt::SmoothTransformation);
	*imgPlane = imgPlane->scaledToHeight(60, Qt::SmoothTransformation);

	/*���ó�������*/
	m_cityLoc = {
		{198 , 415},
		{314 , 305},
		{385 , 203},
		{487 , 125},
		{615 , 73 },
		{424 , 523},
		{454 , 428},
		{537 , 379},
		{564 , 234},
		{462 , 656},
		{606 , 546},
		{634 , 450} };
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
		
		198 , 415	�ɶ�
		314 , 305	����
		385 , 203	̫ԭ
		487 , 125	����	
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
	delete imgMap;
	delete imgBus;	/*������ͼ*/
	delete imgTrain;	/*����ͼ*/
	delete imgPlane;	/*�ɻ���ͼ*/
}

void MapCanvas::UpdateStatus(int transIndex, int progress)
{
	/*����״̬*/
	m_currIndex = transIndex;
	m_currProgress = progress;

	/*����Update()��ͼ*/
	update();
}

void MapCanvas::paintEvent(QPaintEvent* ev)
{
	QPainter p;
	p.begin(this);
	//qDebug() << this->height();

	/*����������Ϣ,������ͼ*/
	QPixmap mapScaled = imgMap->scaledToHeight(this->height(), Qt::SmoothTransformation);/*���߶ȵȱ�������*/
	mapX = this->x() + this->width() / 2 - mapScaled.width() / 2;	/*���µ�ͼ�����Ͻ�����*/
	ratio = mapScaled.height() * 1.0 / originH;						/*���µ�ǰ�����ű���*/
	p.drawPixmap(mapX, 0, mapScaled);
	//for (auto i : m_cityLoc) {
	//	p.drawPixmap(
	//		QPoint(i.x() * ratio + mapX, i.y() * ratio), QPixmap(":/img/bus.png").scaled(20, 20));
	//}

	/*��·��ͼ*/
	if (!m_transList.empty()) {
		p.setRenderHint(QPainter::Antialiasing);
		QPen pen;
		pen.setColor(QColor(236, 28, 36));
		pen.setWidth(10);
		pen.setStyle(Qt::DotLine);
		pen.setCapStyle(Qt::RoundCap);
		pen.setJoinStyle(Qt::RoundJoin);
		p.setPen(pen);
		int xs, ys, xe, ye;
		for (const Transport& t : m_transList) {
			xs = m_cityLoc[t.m_srcIndex].x() * ratio + mapX;/*���x*/
			ys = m_cityLoc[t.m_srcIndex].y() * ratio;		/*���y*/
			xe = m_cityLoc[t.m_destIndex].x() * ratio + mapX;/*�յ�x*/
			ye = m_cityLoc[t.m_destIndex].y() * ratio;		/*�յ�y*/
			p.drawLine(xs, ys, xe, ye);
		}
		/*��·�ߵ�����Ϸ����ͷ*/
		double x1, y1, x2, y2;								/*��ͷ�������������*/
		calcVertexes(xs ,ys, xe, ye, x1, y1, x2, y2);		/*�����ͷ����*/
		pen.setStyle(Qt::SolidLine);						/*ʵ��*/
		p.setPen(pen);
		p.drawLine(xe, ye, x1, y1);							/*����ͷ*/
		p.drawLine(xe, ye, x2, y2);

		if (m_currProgress || m_currIndex) {					/*��ǰ����Ϊ0ʱ�Ҳ��ڳ�����ʱ��������*/
			/*�ڵ�ǰ·���ϻ��ϼ�ͷ��ͿΪ��ɫ*/
			qDebug() << "size " << m_transList.size();
			qDebug() << "index" << m_currIndex;
			xs = m_cityLoc[m_transList.at(m_currIndex).m_srcIndex].x() * ratio + mapX;
			ys = m_cityLoc[m_transList.at(m_currIndex).m_srcIndex].y() * ratio;
			xe = m_cityLoc[m_transList.at(m_currIndex).m_destIndex].x() * ratio + mapX;
			ye = m_cityLoc[m_transList.at(m_currIndex).m_destIndex].y() * ratio;
			/*�������*/
			xe = (xe - xs * 1.0) * m_currProgress / 100 + xs;	/*��ǰ���ȵ��յ�����*/
			ye = (ye - ys * 1.0) * m_currProgress / 100 + ys;
			//calcVertexes(xs, ys, xe, ye, x1, y1, x2, y2);		/*�����ͷ����*/
			//pen.setColor(QColor(0, 174, 49));					/*��ɫ*/
			pen.setStyle(Qt::SolidLine);						/*����*/
			p.setPen(pen);
			p.drawLine(xs, ys, xe, ye);							/*����*/
			//p.drawLine(xe, ye, x1, y1);						/*����ͷ*/
			//p.drawLine(xe, ye, x2, y2);

			/*��ͼ������*/
			switch (m_transList.at(m_currIndex).m_means)
			{
			case Vehicle::bus:
				p.drawPixmap(xe-30, ye-30, *imgBus);
				break;		   		  
			case Vehicle::train:	 
				p.drawPixmap(xe-30, ye-30, *imgTrain);
				break;		   		 
			case Vehicle::plane:	 
				p.drawPixmap(xe-30, ye-30, *imgPlane);
				break;
			default:
				break;
			}
		}
		
	}

	p.end();

}
void MapCanvas::mousePressEvent(QMouseEvent* ev)
{
	qDebug() << ev->x()-mapX << "," << ev->y();
}
/*ˢ��,��ͼ�¼�*/
