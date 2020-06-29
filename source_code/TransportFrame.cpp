#include "TransportFrame.h"
#include "ui_TransportFrame.h"


/*
	Discription: 使用一个transport对象来构建一个行程卡片		
*/
TransportFrame::TransportFrame(const Transport& trans, QWidget* parent)
	:m_trans(trans), QFrame(parent)
{
	ui = new Ui::TransportFrame();
	ui->setupUi(this);
	SetTransport(trans, false);//初始化界面

}

TransportFrame::~TransportFrame()
{
	delete ui;
}

/*
	Discription:		更新卡片中的信息
	Params:
		bool refresh:	为false时仅更新视觉信息
		trans:			一次交通
*/
void TransportFrame::SetTransport(const Transport& trans, bool refresh)
{
	if (refresh) {
		m_trans = trans;
	}
	QString vName = QString::fromStdString(Vehicle::GetAttribute(trans.m_means).name);
	QString dName = QString::fromStdString(trans.m_destCity.m_name);
	QString sName = QString::fromStdString(trans.m_srcCity.m_name);
	ui->vehicle->setText(QString("<center><span style=\"font-size:18pt;\">%1</span></center>").arg(vName));
	ui->destCity->setText(QString("<center><span style=\"font-size:10pt;\">%1</span></center>").arg(dName));
	ui->srcCity->setText(QString("<center><span style=\"font-size:10pt;\">%1</span></center>").arg(sName));
	ui->destTime->setText(QString("<center><span style=\"font-size:10pt;\">%1:00</span></center>").arg(trans.m_endTime));
	ui->srcTime->setText(QString("<center><span style=\"font-size:10pt;\">%1:00</span></center>").arg(trans.m_startTime));
}
