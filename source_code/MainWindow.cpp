#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent),
	settingsWindow(new QWidget(parent)),
	sys(new TransSystem)
	/*gTime(0),
	gDay(0)*/
{
	this->move(400, 60);

	/******************�û�ѡ���ʼ���н���***************************/
	int* originIndex = new int(0);/*���кų�ʼ��*/
	QDialog* chooseDialog = new QDialog(this);/*����*/
	chooseDialog->setWindowTitle(u8"����ģ��ϵͳ");
	QPushButton* closeDialog = new QPushButton(u8"ȷ��",chooseDialog); /*�رհ�ť*/
	QLabel* timeTitle = new QLabel(u8"��ѡ��ʱ�����(��/Сʱ):", chooseDialog);/*����*/
	QComboBox* timeCombo = new QComboBox(chooseDialog); /*ѡ��ʱ���*/
	timeCombo->addItem("1 : 1");
	timeCombo->addItem("2 : 1");
	timeCombo->addItem("5 : 1");
	timeCombo->addItem(u8"10 : 1   ����Ҫ��");
	QLabel* chooseTitle = new QLabel(u8"��ѡ������ǰ���ڵĳ���:", chooseDialog);/*����*/
	QComboBox* cityCombo = new QComboBox(chooseDialog);	/*ѡ����п�*/
	SetCityList(cityCombo, sys->GetCityList());			/*��ʼ�����г����б�*/
	connect(closeDialog, &QPushButton::clicked, [=]() {
		*originIndex = cityCombo->currentIndex();/*��ȡ���е�ֵ*/
		int msec[4] = { 1000, 2000,5000,10000 };
		MS_PER_H =  msec[timeCombo->currentIndex()];	/*��ȡÿСʱ������*/
		chooseDialog->close();					/*�رմ���*/
		});
	QLabel* author = new QLabel("<center style=\"font-family:consolas;font-size:12pt;\"> \
										github.com/tkoniy \
								</center>", chooseDialog);
	/*layout�Ű�*/
	QVBoxLayout* chooseLayout = new QVBoxLayout(chooseDialog);
	chooseLayout->addWidget(timeTitle);
	chooseLayout->addWidget(timeCombo);
	chooseLayout->addWidget(chooseTitle);
	chooseLayout->addWidget(cityCombo);
	chooseLayout->addWidget(closeDialog);
	chooseLayout->addWidget(author);
	/*����ģ̬������������*/
	chooseDialog->exec();

	/**********************************************************************/


	user = new User("USERNAME", *originIndex);	/*���ó�ʼ����*/
	delete originIndex;							/*ɾ������ڴ�*/

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
		ui.btn_planSearch->height()+10,
		ui.btn_planSearch->height()+10));

	this->setWindowTitle(u8"COVID19�ͷ�������ģ��ϵͳ");

	/*���״̬����ǩ+������*/
	currProgress = new QProgressBar(this);
	currSrcCity = new QLabel(this);
	currDestCity = new QLabel(this);
	currTime = new QLabel(this);
	currDay = new QLabel(this);
	currUserStatus = new QLabel(this);
	currProgress->setFont(this->font());
	currSrcCity->setFont(this->font());
	currDestCity->setFont(this->font());
	currTime->setFont(this->font());
	currDay->setFont(this->font());
	currUserStatus->setFont(this->font());
	ui.statusBar->addPermanentWidget(currSrcCity);
	ui.statusBar->addPermanentWidget(new QLabel(">>", this));
	ui.statusBar->addPermanentWidget(currProgress);
	ui.statusBar->addPermanentWidget(new QLabel(">>", this));
	ui.statusBar->addPermanentWidget(currDestCity);
	ui.statusBar->addWidget(currTime);
	ui.statusBar->addWidget(currDay);
	ui.statusBar->addWidget(currUserStatus);
	
	/*��ӵ�ͼҳ���Ű�*/
	mapCanvas = new MapCanvas(ui.page_map);
	QHBoxLayout* pageMapLayout = new QHBoxLayout(ui.page_map);
	pageMapLayout->addWidget(mapCanvas);
	
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
		ui.btn_planConfirm->setText(u8"��Ӽƻ�");
		});

	/*�����ѯ���plan�¼�*/
	connect(ui.btn_planSearch, &QPushButton::clicked, this, &MainWindow::ShowBestPlan);
	
	/*����������plan�¼�*/
	connect(ui.btn_planConfirm, &QPushButton::clicked, [=]() {
		user->SetNewPlanFlag(true);

		///*��ʾ��,��֪�û����³ɹ�*/
		//QMessageBox msg;
		//msg.setIcon(QMessageBox::Icon::Information);
		//msg.setText(
		//	"Your plan is confirmed.\nYou can see your plan in [MyPlan] after it begin.\nYou can modify it before it started.");
		//msg.setStandardButtons(QMessageBox::Ok);
		//msg.setStyleSheet(
		//	"QWidget{background-color: #35353b;}\
		//	 QPushButton{background-color: #4891b4;}\
		//	 QPushButton::hover{background-color: #54aad3;border: 1px solid #46a2da;}\
		//	 QPushButton::pressed{background-color: #2385b4;border: 1px solid #46a2da;}");
		//msg.setWindowFlag(Qt::FramelessWindowHint);
		//msg.exec();
		
		/*����confirm,��д��confirmed*/
		ui.btn_planConfirm->setEnabled(false);
		ui.btn_planConfirm->setText(u8"��ӳɹ�");
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

		/*���³˿�״̬,���µ�ͼ*/
		if (user->GetStatus() == User::status::stay)
			if (user->HaveNewPlan()) {		/*�����ǰû������ִ�еļƻ���cachePlan�мƻ�ʱ*/
				user->UpdatePlan(planCache);/*��ʼִ�л���ļƻ�,״̬���suspend*/
				user->SetNewPlanFlag(false);/*û��newPlan*/

				/*���µ�ͼ�е�plan*/
				mapCanvas->AddPlan(planCache);

			} else if (!mapCanvas->PlanEmpty()) {/*�����ǰû����ִ�еļƻ���mapCanvas��ûͬ��*/
				
				/*���plan*/
				mapCanvas->ClearPlan();
		}

		user->UpdateInfo(sys->GetTime(), sys->GetDay());
		//qDebug() << QString::fromStdString(user->GetStatusName());

		/*ˢ�²���ʾ״̬,��*/
		qDebug() << QString::fromStdString(user->GetStatusName());
		mapCanvas->UpdateStatus(user->GetPlanIndex(), user->GetProgress());
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

void MainWindow::SetTransList(QListWidget* listWidget, bool viewTrueDay, const vector<Transport>& listTrans)
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
		int progress = pastTime * 100 / totalTime;	/*�������*/
		currProgress->setValue(progress);			/*���µ�״̬��*/
		user->SetProgress(progress);				/*���¸��û�*/
		
		/*�޸�bug,����100��ʱ��ǿ�л�һ��ͼ*/
		mapCanvas->UpdateStatus(user->GetPlanIndex(), user->GetProgress());

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
		/*���½��ȸ��û�*/
		user->SetProgress(0);

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
		/*���¸��û�*/
		user->SetProgress(0);

		currDestCity->setText("");
		currSrcCity->setText(QString::fromStdString(
			sys->GetCityList()
			.at(user->GetCityIndex())
			.m_name)
			.toUpper());
	}
	
	/*�������ں�ʱ��*/
	currTime->setText(QString("%1:00").arg(sys->GetTime(), 2, 10, QLatin1Char('0')));
	currDay->setText(QString(u8"��%1��").arg(sys->GetDay()));
	currUserStatus->setText(QString(" - %1 - ").arg(QString::fromStdString(user->GetStatusName())));

}

/*����travelҳ*/
void MainWindow::UpdatePageTravel()
{
	/*�����µ����мƻ���ʼ�ص������ START FROM ������ֻ�ṩһ��ѡ��(��ʱ)*/
	ui.combo_srcTravel->clear();
	if (User::status::stay == user->GetStatus()) {
		/*����û�û����������,��ô��һ�����е���ʼ�ص�Ϊ�û�λ��*/
		//qDebug() << QString::fromStdString(user->GetStatusName());
		ui.combo_srcTravel->addItem(
			QString::fromStdString(
				sys->GetCityList()
				.at(user->GetCityIndex())
				.m_name));
	} else {
		/*����û���������,��ô������ʼ�ص�Ϊ��ǰ���мƻ����յ����*/
		//qDebug() << user->GetPlan().size();
		int srcIndex = user->GetPlan().at(user->GetPlan().size() - 1).m_destIndex;
		City nextSrc = sys->GetCityList().at(srcIndex);
		ui.combo_srcTravel->addItem(QString::fromStdString(nextSrc.m_name));
	}

	/*����֮ǰ�ѵļƻ������ˣ���Ӧ����confirm�����Խ���confirm��ť*/
	if (ui.btn_planConfirm->isEnabled()) {
		ui.btn_planConfirm->setEnabled(false);
		ui.btn_planConfirm->setText(u8"�ƻ��ѹ���"); /*��ʾ����*/
	}


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
	if (TransSystem::CompareDateTimeL(endTime, startTime, endDay, startDay)) {/*ֻ���ں��������²ż���*/
		planCache = sys->FindPlan(
			srcIndex, destIndex, startDay, endDay, startTime, endTime,
			ui_settings.check_repVisit->isChecked(),		/*��ȡ�û��Ƿ�����ʱ*/
			ui_settings.check_limTime->isChecked(),			/*��ȡ�û��Ƿ������ظ�����*/
			ui_settings.check_transRisk->isChecked());		/*��ȡ�û��Ƿ��ǽ�ͨ���ߵķ���*/
		SetTransList(ui.listWidget_plan, true, planCache);	/*����չ����travelҳ��*/
	} else {

		/*��ʾ��ʾ��,������ʽ*/
		QMessageBox msg(this);
		msg.setIcon(QMessageBox::Icon::Critical);
		msg.setText(QString(u8"��������ʱ�����������\n�� %1 �� %2:00 ��").arg(startDay).arg(startTime));
		msg.setFont(this->font());
		QPushButton btn(u8"��֪����",&msg);
		btn.setFont(this->font());
		msg.addButton(&btn,QMessageBox::YesRole);
		msg.setStyleSheet(
			"QWidget{background-color: #35353b;}\
			 QPushButton{background-color: #4891b4;}\
			 QPushButton::hover{background-color: #54aad3;border: 1px solid #46a2da;}\
			 QPushButton::pressed{background-color: #2385b4;border: 1px solid #46a2da;}");
		msg.setWindowFlag(Qt::FramelessWindowHint);
		msg.exec();
	}

	

	/*����confirm��ť��״̬*/
	if (planCache.empty()) {/*�����ȡ���ļƻ��ǿյ�,���������Ϊ�޽�*/
		ui.btn_planConfirm->setEnabled(false);	/*�������û�ȷ���г�*/
		ui.btn_planConfirm->setText(u8"δ�������ƻ�");
	} else{
		ui.btn_planConfirm->setEnabled(true);	/*�����û�ȷ���г�*/
		ui.btn_planConfirm->setText(u8"��Ӽƻ�");
	}
}

void MainWindow::OnSettingChanged(int state)
{
	/*����ʱ������£�������ѡ�������ظ�����*/
	if(!ui_settings.check_limTime->isChecked()) {/*��ʱû��*/
		ui_settings.check_repVisit->setEnabled(false);	/*���õ�����*/
		ui_settings.check_repVisit->setChecked(false);	/*ȡ����������*/
	} else {
		ui_settings.check_repVisit->setEnabled(true);	/*�������������ѡ�������ظ�*/
	}
	
}
