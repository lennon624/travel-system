#include "MapCanvas.h"

MapCanvas::MapCanvas(QWidget *parent)
	: QWidget(parent),
	imgMap(new QPixmap(":/img/map.png"))
{
	ui.setupUi(this);
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
		
		487 , 125	北京
		198 , 415	成都
		314 , 305	西安
		385 , 203	太原
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
}

void MapCanvas::paintEvent(QPaintEvent* ev)
{
	QPainter p;
	p.begin(this);
	QPixmap mapScaled = imgMap->scaledToHeight(this->height(), Qt::SmoothTransformation);/*按高度等比例缩放*/
	mapX = this->x() + this->width() / 2 - mapScaled.width() / 2;	/*更新地图的左上角坐标*/
	ratio = mapScaled.height() * 1.0 / originH;						/*更新当前的缩放比例*/
	p.drawPixmap(mapX, 0, mapScaled);
	p.end();

}
void MapCanvas::mousePressEvent(QMouseEvent* ev)
{
	qDebug() << ev->x()-mapX << "," << ev->y();
}
/*刷新,画图事件*/
