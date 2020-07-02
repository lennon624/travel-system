#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent),
	user(new User("USERNAME", City())),
	sys(new TransSystem)
	/*gTime(0),
	gDay(0)*/
{
	/*��ʼ��ui����*/
	ui.setupUi(this);
	
	/*���״̬����ǩ*/
	currProgress = new QProgressBar(this);
	currSrcCity = new QLabel(this);
	currDestCity = new QLabel(this);
	currTime = new QLabel(this);
	currDay = new QLabel(this);
	currUserStatus = new QLabel(this);
	ui.statusBar->addPermanentWidget(currSrcCity);
	ui.statusBar->addPermanentWidget(currProgress);
	ui.statusBar->addPermanentWidget(currDestCity);
	ui.statusBar->addWidget(currTime);
	ui.statusBar->addWidget(currDay);
	ui.statusBar->addWidget(currUserStatus);

	/*debug ����user״̬*/
	user->UpdatePlan({
		
		Transport(
		sys->GetCityList()[0],
		sys->GetCityList()[2],
		Vehicle::bus,5,10,0,2),

		Transport(
		sys->GetCityList()[2],
		sys->GetCityList()[1],
		Vehicle::bus,0,15,3,5) 
		}
	); /*�ӳ���0������2,  0:00��10:00*/
	/*debug ��ϵͳʱ�������3*/
	user->UpdateInfo(sys->GetTime(),sys->GetDay());


	/*��ʼ������ҳ��*/
	
	/*��ʼ��״̬����ǩ*/
	UpdateStatusBar();

	/*��ʼ�������б�*/  /*TODO: ����combo box*/
	/*SetCityList(ui.listWidget_src, sys->GetCityList());
	SetCityList(ui.listWidget_dest, sys->GetCityList());*/
	SetCityList(ui.combo_srcCity, sys->GetCityList());
	SetCityList(ui.combo_destCity, sys->GetCityList());
	SetCityList(ui.combo_destTravel, sys->GetCityList());
	SetCityList(ui.combo_srcTravel, { user->GetCity() });
	

	/*��ʼ����*/
	hTimerId = startTimer(MS_PER_H);

	/*�����ź�*/
	/*�����л�ҳ���¼�*/
	connect(ui.btn_pMap, &QPushButton::clicked, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.page_map);
		});
	connect(ui.btn_pTravel, &QPushButton::clicked, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.page_travel);
		});
	connect(ui.btn_pTrans, &QPushButton::clicked, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.page_trans);
		});

	/*��������������¼�*/
	//connect(ui.listWidget_dest, &QListWidget::itemSelectionChanged, [=]() {
	//	qDebug() << QString::fromStdString(sys->GetCityList()[ui.listWidget_dest->currentRow()].m_name);
	//	//��������������֮��ĳ��� setTransList
	//	
	//	});
	//connect(ui.listWidget_src, &QListWidget::itemSelectionChanged, [=]() {
	//	qDebug() << QString::fromStdString(sys->GetCityList()[ui.listWidget_src->currentRow()].m_name);
	//	//��������������֮��ĳ��� setTransList
	//	});

	/*����timeTable��ѯҳ��ʼ���и����¼�,ֻ�ܷ����*/
	connect(ui.combo_srcCity, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int srcIndex) {
		qDebug() << QString::fromStdString(sys->GetCityList().at(srcIndex).m_name);

		/*TODO: �ж��û�ѡ��Ľ�ͨ��ʽ*/

		int destIndex = ui.combo_destCity->currentIndex();/*��ȡ��ʼ��������*/
		SetTransList(ui.listWidget_trans, sys->GetTransList(srcIndex, destIndex, Vehicle::all));/*�޸�ʱ�̱�*/
		});
	/*�����յ���и����¼�*/
	connect(ui.combo_destCity, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int destIndex) {
		qDebug() << QString::fromStdString(sys->GetCityList().at(destIndex).m_name);

		/*TODO: �ж��û�ѡ��Ľ�ͨ��ʽ*/

		int srcIndex = ui.combo_srcCity->currentIndex();/*��ȡ��ʼ��������*/
		SetTransList(ui.listWidget_trans, sys->GetTransList(srcIndex, destIndex, Vehicle::all));/*�޸�ʱ�̱�*/
		});

	connect(ui.btn_planMy, &QPushButton::clicked, [=]() {
		});

}

MainWindow::~MainWindow() {
	delete sys;
	delete user;
}

void MainWindow::timerEvent(QTimerEvent* ev)
{
	/*����ʱ�Ӵ���״̬*/
	if (hTimerId == ev->timerId()) {
		/*����ϵͳ����*/
		sys->SetTimeUp();
		/*����״̬��*/
		UpdateStatusBar();

		/*���³˿�״̬*/
		if (user->GetStatus() == User::status::stay
			&& user->HaveNewPlan()) {	/*�����ǰû������ִ�еļƻ���cachePlan�мƻ�ʱ*/
			user->UpdatePlan(planCache);/*��ʼִ�л���ļƻ�*/
			user->SetNewPlanFlag(false);/*û��newPlan*/
		}
		user->UpdateInfo(sys->GetTime(), sys->GetDay());
		qDebug() << QString::fromStdString(user->GetStatusName());

		/*����travelҳ*/
		UpdatePageTravel();
	}
}



void MainWindow::SetCityList(QComboBox* comboBox, const vector<City>& listCity)
{
	for (const City& city : listCity) {
		comboBox->addItem(QString::fromStdString(city.m_name));
	}
}

void MainWindow::SetTransList(QListWidget* listWidget, const vector<Transport>& listTrans)
{
	//disconnect
	listWidget->disconnect();
	//clear
	listWidget->clear();
	for (const Transport& trans : listTrans) {
		QListWidgetItem* item = new QListWidgetItem(listWidget);
		TransportFrame *f = new TransportFrame(trans,listWidget);
		item->setSizeHint(f->size());
		listWidget->setItemWidget(item, f);
	}
	//connect����
}

/*
	����״̬��
*/
void MainWindow::UpdateStatusBar()
{

	/*���ý��ȿؼ�*/
	if (User::status::on == user->GetStatus()) {
		/*���㲢���õ�ǰtransport�Ľ��� */
		currProgress->setDisabled(false);
		/*���ý���*/
		int totalTime = TransSystem::CountTime(
			user->GetTransport().m_startTime,
			user->GetTransport().m_endTime,
			user->GetTransport().m_startDay,
			user->GetTransport().m_endDay);
		int pastTime = TransSystem::CountTime(
			user->GetTransport().m_startTime,
			sys->GetTime(),
			user->GetTransport().m_startDay,
			sys->GetDay()
		);
		currProgress->setValue(pastTime * 100 / totalTime);

		/*������ʼ��Ŀ�ĳ���*/
		currSrcCity->setText(
			QString::fromStdString(user->GetTransport().m_srcCity.m_name)
		);
		currDestCity->setText(
			QString::fromStdString(user->GetTransport().m_destCity.m_name)
		);
	} else if (User::status::suspend == user->GetStatus()){
		/*���õ�ǰtransport�Ľ���Ϊ0*/
		currProgress->setDisabled(false);
		currProgress->setValue(0);

		/*������ʼ��Ŀ�ĳ���*/
		currSrcCity->setText(
			QString::fromStdString(user->GetTransport().m_srcCity.m_name)
		);
		currDestCity->setText(
			QString::fromStdString(user->GetTransport().m_destCity.m_name)
		);
	} else if (User::status::stay == user->GetStatus()) {
		/*���õ�ǰtransport�Ľ���Ϊ0�ҽ��ý�����*/
		currProgress->setDisabled(true);
		currProgress->setValue(0);

		currDestCity->setText("NONE");
		currSrcCity->setText(QString::fromStdString(user->GetCity().m_name).toUpper());
	}
	
	/*�������ں�ʱ��*/
	currTime->setText(QString("%1:00").arg(sys->GetTime(), 2, 10, QLatin1Char('0')));
	currDay->setText(QString("Day %1").arg(sys->GetDay()));
	currUserStatus->setText(QString(" --- status: %1 --- ").arg(QString::fromStdString(user->GetStatusName())));

}

/*����travelҳ*/
void MainWindow::UpdatePageTravel()
{
	/*�����µ����мƻ���ʼ�ص������ START FROM ������ֻ�ṩһ��ѡ��(��ʱ)*/
	ui.combo_srcTravel->clear();
	if (User::status::stay == user->GetStatus()) {
		/*����û�û����������,��ô��һ�����е���ʼ�ص�Ϊ�û�λ��*/
		qDebug() << QString::fromStdString(user->GetStatusName());
		ui.combo_srcTravel->addItem(QString::fromStdString(user->GetCity().m_name));
	} else {
		/*����û���������,��ô������ʼ�ص�Ϊ��ǰ���мƻ����յ����*/
		qDebug() << user->GetPlan().size();
		City nextSrc = user->GetPlan().at(user->GetPlan().size() - 1).m_destCity;
		ui.combo_srcTravel->addItem(QString::fromStdString(nextSrc.m_name));
	}

	/*������ͬ������(���˲�����)*/
}
