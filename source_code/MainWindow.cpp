#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent),
	settingsWindow(new QWidget(parent)),
	user(new User("USERNAME", User::DEFAULT_CITY_INDEX)),
	sys(new TransSystem)
	/*gTime(0),
	gDay(0)*/
{
	/*��ʼ��ui����*/
	ui.setupUi(this);/*������*/
	ui_settings.setupUi(settingsWindow);/*���ô���*/
	settingsWindow->setWindowFlag(Qt::WindowType::FramelessWindowHint);
	settingsWindow->setStyleSheet(
		"QWidget{background-color: #35353b;}\
		 QPushButton{background-color: #4891b4;}\
		 QPushButton::hover{background-color: #54aad3;border: 1px solid #46a2da;}\
		 QPushButton::pressed{background-color: #2385b4;border: 1px solid #46a2da;}");

	/*����һ��setting��ť�ĳߴ�*/
	ui.btn_planSetting->setFixedSize(QSize(
		ui.btn_planSearch->height()+9,
		ui.btn_planSearch->height()+9));


	/*���״̬����ǩ*/
	currProgress = new QProgressBar(this);
	currSrcCity = new QLabel(this);
	currDestCity = new QLabel(this);
	currTime = new QLabel(this);
	currDay = new QLabel(this);
	currUserStatus = new QLabel(this);
	ui.statusBar->addPermanentWidget(currSrcCity);
	ui.statusBar->addPermanentWidget(new QLabel(">>", this));
	ui.statusBar->addPermanentWidget(currProgress);
	ui.statusBar->addPermanentWidget(new QLabel(">>", this));
	ui.statusBar->addPermanentWidget(currDestCity);
	ui.statusBar->addWidget(currTime);
	ui.statusBar->addWidget(currDay);
	ui.statusBar->addWidget(currUserStatus);
	
	///*debug ����user״̬*/
	//user->UpdatePlan({
	//	
	//	Transport(
	//	0,
	//	2,
	//	Vehicle::bus,5,10,0,2),

	//	Transport(
	//	2,
	//	1,
	//	Vehicle::bus,0,15,3,5) 
	//	}
	//); /*�ӳ���0������2,  0:00��10:00*/
	///*debug ��ϵͳʱ�������3*/
	//user->UpdateInfo(sys->GetTime(),sys->GetDay());


	/*��ʼ������ҳ��*/
	
	/*��ʼ��״̬����ǩ*/
	UpdateStatusBar();

	/*��ʼ�������б�*/  /*TODO: ����combo box*/
	/*SetCityList(ui.listWidget_src, sys->GetCityList());
	SetCityList(ui.listWidget_dest, sys->GetCityList());*/
	SetCityList(ui.combo_srcCity, sys->GetCityList());
	SetCityList(ui.combo_destCity, sys->GetCityList());
	SetCityList(ui.combo_destTravel, sys->GetCityList());
	SetCityList(ui.combo_srcTravel, { sys->GetCityList().at(user->GetCityIndex()) });
	

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
		/*TODO: �ж��û�ѡ��Ľ�ͨ��ʽ*/

		int destIndex = ui.combo_destCity->currentIndex();/*��ȡ��ʼ��������*/
		SetTransList(ui.listWidget_trans, false, sys->GetTransList(srcIndex, destIndex, Vehicle::all));/*�޸�ʱ�̱�*/
		});

	/*�����յ���и����¼�*/
	connect(ui.combo_destCity, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int destIndex) {
		/*TODO: �ж��û�ѡ��Ľ�ͨ��ʽ*/

		int srcIndex = ui.combo_srcCity->currentIndex();/*��ȡ��ʼ��������*/
		SetTransList(ui.listWidget_trans,false, sys->GetTransList(srcIndex, destIndex, Vehicle::all));/*�޸�ʱ�̱�*/
		});

	/*�����ѯ��ǰ����ִ�е�plan�¼�*/
	connect(ui.btn_planMy, &QPushButton::clicked, [=]() {
		SetTransList(ui.listWidget_plan,true, user->GetPlan());
		/*�л���myPlan֮���������������,Ҳ����confirm*/
		ui.btn_planConfirm->setEnabled(false);
		});

	/*�����ѯ���plan�¼�*/
	connect(ui.btn_planSearch, &QPushButton::clicked, this, &MainWindow::ShowBestPlan);
	
	/*����������plan�¼�*/
	connect(ui.btn_planConfirm, &QPushButton::clicked, [=]() {
		user->SetNewPlanFlag(true);
		ui.btn_planConfirm->setEnabled(false);
		});

	/*�������͹ر�����ҳ���¼�*/
	connect(ui.btn_planSetting, &QPushButton::clicked, [=]() {
		if (settingsWindow->isVisible()) {
			settingsWindow->setVisible(false);
		} else {
			settingsWindow->setVisible(true);
		}
		});
	
	connect(ui_settings.btn_close, &QPushButton::clicked, [=]() {
		settingsWindow->setVisible(false);
		});

	/*����settings���޸��¼�*/
	connect(ui_settings.check_limTime, &QCheckBox::stateChanged, this, &MainWindow::OnSettingChanged);
	connect(ui_settings.check_repVisit, &QCheckBox::stateChanged, this, &MainWindow::OnSettingChanged);
	connect(ui_settings.check_transRisk, &QCheckBox::stateChanged, this, &MainWindow::OnSettingChanged);
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

void MainWindow::closeEvent(QCloseEvent* ev)
{
	settingsWindow->close();/*˳��Ѹ������ڹص�*/
}



void MainWindow::SetCityList(QComboBox* comboBox, const vector<City>& listCity)
{
	for (const City& city : listCity) {
		comboBox->addItem(QString::fromStdString(city.m_name));
	}
}

void MainWindow::SetTransList(QListWidget* listWidget,bool viewTrueDay, const vector<Transport>& listTrans)
{
	//disconnect
	listWidget->disconnect();
	//clear
	listWidget->clear();
	for (const Transport& trans : listTrans) {
		QListWidgetItem* item = new QListWidgetItem(listWidget);
		TransportFrame* f = new TransportFrame(trans, *sys, viewTrueDay, listWidget);
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
			QString::fromStdString(
				sys->GetCityList()
				.at(user->GetTransport().m_srcIndex)
				.m_name).toUpper()
		);
		currDestCity->setText(
			QString::fromStdString(
				sys->GetCityList()
				.at(user->GetTransport().m_destIndex)
				.m_name).toUpper()
		);
	} else if (User::status::suspend == user->GetStatus()){
		/*���õ�ǰtransport�Ľ���Ϊ0*/
		currProgress->setDisabled(false);
		currProgress->setValue(0);

		/*������ʼ��Ŀ�ĳ���*/
		currSrcCity->setText(
			QString::fromStdString(
				sys->GetCityList()
				.at(user->GetTransport().m_srcIndex)
				.m_name).toUpper()
		);
		currDestCity->setText(
			QString::fromStdString(
				sys->GetCityList()
				.at(user->GetTransport().m_destIndex)
				.m_name).toUpper()
		);
	} else if (User::status::stay == user->GetStatus()) {
		/*���õ�ǰtransport�Ľ���Ϊ0�ҽ��ý�����*/
		currProgress->setDisabled(true);
		currProgress->setValue(0);

		currDestCity->setText("NONE");
		currSrcCity->setText(QString::fromStdString(
			sys->GetCityList()
			.at(user->GetCityIndex())
			.m_name)
			.toUpper());
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
		ui.combo_srcTravel->addItem(
			QString::fromStdString(
				sys->GetCityList()
				.at(user->GetCityIndex())
				.m_name));
	} else {
		/*����û���������,��ô������ʼ�ص�Ϊ��ǰ���мƻ����յ����*/
		qDebug() << user->GetPlan().size();
		int srcIndex = user->GetPlan().at(user->GetPlan().size() - 1).m_destIndex;
		City nextSrc = sys->GetCityList().at(srcIndex);
		ui.combo_srcTravel->addItem(QString::fromStdString(nextSrc.m_name));
	}

	/*����֮ǰ�ѵļƻ������ˣ���Ӧ����confirm�����Խ���confirm��ť*/
	ui.btn_planConfirm->setEnabled(false);

	/*������ͬ������(���˲�����)*/
}

/*��ѯ����ʾ���plan*/
void MainWindow::ShowBestPlan()
{
	/*���㵱ǰ�г̽���������ڳ���,��plan����ʼ����*/
	int srcIndex = user->GetCityIndex();			/*Ĭ��Ϊ��ǰ����*/
	if (User::status::stay != user->GetStatus()) {	/*�����ǰ����ִ�мƻ�,��ѡ��ƻ����յ����*/
		srcIndex = user->GetPlan().at(user->GetPlan().size() - 1).m_destIndex;
	}

	/*��ȡplan��Ŀ�ĳ���*/
	int destIndex = ui.combo_destTravel->currentIndex();

	/*��ȡ��ʼʱ�������*/
	int startTime = sys->GetTime();	/*Ĭ��Ϊ��ǰʱ��*/
	int startDay = sys->GetDay();	/*Ĭ��Ϊ��ǰ����*/
	if (User::status::stay != user->GetStatus()) {	/*�����ǰ����ִ�мƻ�,��ѡ��ƻ����յ�ʱ��*/
		startTime = user->GetPlan().at(user->GetPlan().size() - 1).m_endTime;
		startDay = user->GetPlan().at(user->GetPlan().size() - 1).m_endDay;
	}

	/*��ȡ������ʱ�������*/
	int endTime = ui.spinBox_endTime->value();
	int endDay = ui.spinBox_endDay->value();

	/*-----------------------������ѷ���-----------------------------*/
	//qDebug() << ui_settings.check_limTime->isChecked();

	planCache = sys->FindPlan(
		srcIndex, destIndex, startDay, endDay, startTime, endTime,
		ui_settings.check_repVisit->isChecked(),		/*��ȡ�û��Ƿ�����ʱ*/
		ui_settings.check_limTime->isChecked(),			/*��ȡ�û��Ƿ������ظ�����*/
		ui_settings.check_transRisk->isChecked());		/*��ȡ�û��Ƿ��ǽ�ͨ���ߵķ���*/
	SetTransList(ui.listWidget_plan, true, planCache);	/*����չ����travelҳ��*/

	

	/*����confirm��ť��״̬*/
	if (planCache.empty()) {/*�����ȡ���ļƻ��ǿյ�,���������Ϊ�޽�*/
		ui.btn_planConfirm->setEnabled(false);	/*�������û�ȷ���г�*/
	} else{
		ui.btn_planConfirm->setEnabled(true);	/*�����û�ȷ���г�*/
	}
}

void MainWindow::OnSettingChanged(int state)
{
	/*����ʱҲ�����ǽ�ͨ��ʽ�ķ��յ�����£�������ѡ�������ظ�����*/

	/*�����һ�ڶ���checkbox������һ����ȡ����ʱ��,�鿴��һ��,���Ҳ��ȡ����,�Ǿ����õ�����ѡ��Ϊdisable��false*/
	/*���������,���õ��������Ϊenable*/
	if(!ui_settings.check_limTime->isChecked()			
		&& !ui_settings.check_transRisk->isChecked()) {/*������û�д�*/
		ui_settings.check_repVisit->setEnabled(false);	/*���õ�����*/
		ui_settings.check_repVisit->setChecked(false);	/*ȡ����������*/
	} else {
		ui_settings.check_repVisit->setEnabled(true);	/*�������������ѡ�������ظ�*/
	}
	
}
