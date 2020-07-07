#include "TransportFrame.h"
#include "ui_TransportFrame.h"


/*
	Discription: 使用一个transport对象来构建一个行程卡片		
*/
TransportFrame::TransportFrame(
	const Transport& trans, const TransSystem& sys, bool viewTrueDay, QWidget* parent)
	:m_trans(trans), QFrame(parent)
{
	ui = new Ui::TransportFrame();
	ui->setupUi(this);
	SetTransport(trans, sys, viewTrueDay, false);//初始化界面,好像没啥区别了,因为很少直接修改这个

}

TransportFrame::~TransportFrame()
{
	delete ui;
	qDebug() << "tf deleted";
}

/*
	Discription:		更新卡片中的信息
	Params:
		bool refresh:	为false时仅更新视觉信息
		trans:			一次交通
		sys:			交通系统，主要获取城市信息
*/

/*viewTrueDay参数：是否展示确定的日期*/
void TransportFrame::SetTransport(const Transport& trans, const TransSystem& sys, bool viewTrueDay, bool refresh)
{
	if (refresh) {
		m_trans = trans;
	}
	QString vName = QString::fromStdString(Vehicle::GetAttribute(trans.m_means).name);
	QString dName = QString::fromStdString(sys.GetCityList().at(trans.m_destIndex).m_name);
	QString sName = QString::fromStdString(sys.GetCityList().at(trans.m_srcIndex).m_name);
	ui->vehicle->setText(QString("<center><span style=\"font-size:18pt;\">%1</span></center>").arg(vName));
	ui->destCity->setText(QString("<center><span style=\"font-size:14pt;\">%1</span></center>").arg(dName));
	ui->srcCity->setText(QString("<center><span style=\"font-size:14pt;\">%1</span></center>").arg(sName));
	
	if (viewTrueDay) {
		ui->srcTime->setText(
			QString(u8"<center><span style=\"font-size:12pt;\">第%1天%2:00</span></center>")
			.arg(trans.m_startDay)
			.arg(trans.m_startTime));
		ui->destTime->setText(
			QString(u8"<center><span style=\"font-size:12pt;\">第%1天%2:00 </span></center>")
			.arg(trans.m_endDay)
			.arg(trans.m_endTime));
	} else {
		ui->srcTime->setText(
			QString(u8"<center><span style=\"font-size:12pt;\">%1:00</span></center>")
			.arg(trans.m_startTime));
		ui->destTime->setText(
			QString(u8"<center><span style=\"font-size:12pt;\">%1:00 +%2</span></center>")
			.arg(trans.m_endTime)
			.arg(trans.m_endDay - trans.m_startDay));
	}
	
}
