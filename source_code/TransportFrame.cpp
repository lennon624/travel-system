#include "TransportFrame.h"
#include "ui_TransportFrame.h"


/*
	Discription: ʹ��һ��transport����������һ���г̿�Ƭ		
*/
TransportFrame::TransportFrame(
	const Transport& trans, const TransSystem& sys, bool viewTrueDay, QWidget* parent)
	:m_trans(trans), QFrame(parent)
{
	ui = new Ui::TransportFrame();
	ui->setupUi(this);
	SetTransport(trans, sys, viewTrueDay, false);//��ʼ������,����ûɶ������,��Ϊ����ֱ���޸����

}

TransportFrame::~TransportFrame()
{
	delete ui;
	qDebug() << "tf deleted";
}

/*
	Discription:		���¿�Ƭ�е���Ϣ
	Params:
		bool refresh:	Ϊfalseʱ�������Ӿ���Ϣ
		trans:			һ�ν�ͨ
		sys:			��ͨϵͳ����Ҫ��ȡ������Ϣ
*/

/*viewTrueDay�������Ƿ�չʾȷ��������*/
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
			QString(u8"<center><span style=\"font-size:12pt;\">��%1��%2:00</span></center>")
			.arg(trans.m_startDay)
			.arg(trans.m_startTime));
		ui->destTime->setText(
			QString(u8"<center><span style=\"font-size:12pt;\">��%1��%2:00 </span></center>")
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
