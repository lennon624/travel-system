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

	/*缩放图标*/
	*imgBus = imgBus->scaledToHeight(60, Qt::SmoothTransformation);
	*imgTrain = imgTrain->scaledToHeight(60, Qt::SmoothTransformation);
	*imgPlane = imgPlane->scaledToHeight(60, Qt::SmoothTransformation);

	/*设置城市坐标*/
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
	/*思路*/
	/*
		初始的pixmap就要先缩放成窗口开始的状态，并保存当前的尺寸。
		使用鼠标获得初始窗口大小时的所有坐标(相对于图片的左上角)
		写一个CityLocs数组，把坐标手动录进去。
		设置一个全局变量double ratio。
		每次刷新屏幕后，计算当前相对于初始的ratio，和起始点x，更新进去。
		
		每次画图的时候，计算城市坐标的公式为:
			x[i] = mapX + CityLocs[i].x*ratio
			y[i] = CityLocs[i].y * ratio
		初始为753,753;
		
		198 , 415	成都
		314 , 305	西安
		385 , 203	太原
		487 , 125	北京	
		615 , 73	沈阳
		424 , 523	长沙
		454 , 428	武汉
		537 , 379	合肥
		564 , 234	济南
		462 , 656	广州
		606 , 546	福州
		634 , 450	杭州

	*/
}

MapCanvas::~MapCanvas()
{
	delete imgMap;
	delete imgBus;	/*汽车贴图*/
	delete imgTrain;	/*火车贴图*/
	delete imgPlane;	/*飞机贴图*/
}

void MapCanvas::UpdateStatus(int transIndex, int progress)
{
	/*更新状态*/
	m_currIndex = transIndex;
	m_currProgress = progress;

	/*调用Update()画图*/
	update();
}

void MapCanvas::paintEvent(QPaintEvent* ev)
{
	QPainter p;
	p.begin(this);
	//qDebug() << this->height();

	/*更新坐标信息,画背景图*/
	QPixmap mapScaled = imgMap->scaledToHeight(this->height(), Qt::SmoothTransformation);/*按高度等比例缩放*/
	mapX = this->x() + this->width() / 2 - mapScaled.width() / 2;	/*更新地图的左上角坐标*/
	ratio = mapScaled.height() * 1.0 / originH;						/*更新当前的缩放比例*/
	p.drawPixmap(mapX, 0, mapScaled);
	//for (auto i : m_cityLoc) {
	//	p.drawPixmap(
	//		QPoint(i.x() * ratio + mapX, i.y() * ratio), QPixmap(":/img/bus.png").scaled(20, 20));
	//}

	/*画路线图*/
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
			xs = m_cityLoc[t.m_srcIndex].x() * ratio + mapX;/*起点x*/
			ys = m_cityLoc[t.m_srcIndex].y() * ratio;		/*起点y*/
			xe = m_cityLoc[t.m_destIndex].x() * ratio + mapX;/*终点x*/
			ye = m_cityLoc[t.m_destIndex].y() * ratio;		/*终点y*/
			p.drawLine(xs, ys, xe, ye);
		}
		/*在路线的最后画上方向箭头*/
		double x1, y1, x2, y2;								/*箭头的两个点的坐标*/
		calcVertexes(xs ,ys, xe, ye, x1, y1, x2, y2);		/*计算箭头坐标*/
		pen.setStyle(Qt::SolidLine);						/*实线*/
		p.setPen(pen);
		p.drawLine(xe, ye, x1, y1);							/*画箭头*/
		p.drawLine(xe, ye, x2, y2);

		if (m_currProgress || m_currIndex) {					/*当前进度为0时且不在出发点时画进度线*/
			/*在当前路线上画上箭头并涂为绿色*/
			qDebug() << "size " << m_transList.size();
			qDebug() << "index" << m_currIndex;
			xs = m_cityLoc[m_transList.at(m_currIndex).m_srcIndex].x() * ratio + mapX;
			ys = m_cityLoc[m_transList.at(m_currIndex).m_srcIndex].y() * ratio;
			xe = m_cityLoc[m_transList.at(m_currIndex).m_destIndex].x() * ratio + mapX;
			ye = m_cityLoc[m_transList.at(m_currIndex).m_destIndex].y() * ratio;
			/*计算进度*/
			xe = (xe - xs * 1.0) * m_currProgress / 100 + xs;	/*当前进度的终点坐标*/
			ye = (ye - ys * 1.0) * m_currProgress / 100 + ys;
			//calcVertexes(xs, ys, xe, ye, x1, y1, x2, y2);		/*计算箭头坐标*/
			//pen.setColor(QColor(0, 174, 49));					/*绿色*/
			pen.setStyle(Qt::SolidLine);						/*虚线*/
			p.setPen(pen);
			p.drawLine(xs, ys, xe, ye);							/*画线*/
			//p.drawLine(xe, ye, x1, y1);						/*画箭头*/
			//p.drawLine(xe, ye, x2, y2);

			/*贴图啦啦啦*/
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
/*刷新,画图事件*/
