#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent),
	settingsWindow(new QWidget(parent)),
	user(new User("USERNAME", User::DEFAULT_CITY_INDEX)),
	sys(new TransSystem)
	/*gTime(0),
	gDay(0)*/
{
	/*初始化ui界面*/
	ui.setupUi(this);/*主窗口*/
	ui_settings.setupUi(settingsWindow);/*设置窗口*/
	settingsWindow->setWindowFlag(Qt::WindowType::FramelessWindowHint);
	settingsWindow->setStyleSheet(
		"QWidget{background-color: #35353b;}\
		 QPushButton{background-color: #4891b4;}\
		 QPushButton::hover{background-color: #54aad3;border: 1px solid #46a2da;}\
		 QPushButton::pressed{background-color: #2385b4;border: 1px solid #46a2da;}");

	/*调整一下setting按钮的尺寸*/
	ui.btn_planSetting->setFixedSize(QSize(
		ui.btn_planSearch->height()+9,
		ui.btn_planSearch->height()+9));


	/*添加状态栏标签*/
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
	
	///*debug 设置user状态*/
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
	//); /*从城市0到城市2,  0:00到10:00*/
	///*debug 把系统时间调整到3*/
	//user->UpdateInfo(sys->GetTime(),sys->GetDay());


	/*初始化所有页面*/
	
	/*初始化状态栏标签*/
	UpdateStatusBar();

	/*初始化城市列表*/  /*TODO: 改用combo box*/
	/*SetCityList(ui.listWidget_src, sys->GetCityList());
	SetCityList(ui.listWidget_dest, sys->GetCityList());*/
	SetCityList(ui.combo_srcCity, sys->GetCityList());
	SetCityList(ui.combo_destCity, sys->GetCityList());
	SetCityList(ui.combo_destTravel, sys->GetCityList());
	SetCityList(ui.combo_srcTravel, { sys->GetCityList().at(user->GetCityIndex()) });
	

	/*开始工作*/
	hTimerId = startTimer(MS_PER_H);

	/*处理信号*/
	/*处理切换页面事件*/
	connect(ui.btn_pMap, &QPushButton::clicked, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.page_map);
		});
	connect(ui.btn_pTravel, &QPushButton::clicked, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.page_travel);
		});
	connect(ui.btn_pTrans, &QPushButton::clicked, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.page_trans);
		});

	/*处理鼠标点击城市事件*/
	//connect(ui.listWidget_dest, &QListWidget::itemSelectionChanged, [=]() {
	//	qDebug() << QString::fromStdString(sys->GetCityList()[ui.listWidget_dest->currentRow()].m_name);
	//	//更新这两个城市之间的车次 setTransList
	//	
	//	});
	//connect(ui.listWidget_src, &QListWidget::itemSelectionChanged, [=]() {
	//	qDebug() << QString::fromStdString(sys->GetCityList()[ui.listWidget_src->currentRow()].m_name);
	//	//更新这两个城市之间的车次 setTransList
	//	});

	/*处理timeTable查询页起始城市更换事件,只能放最后*/
	connect(ui.combo_srcCity, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int srcIndex) {
		/*TODO: 判断用户选择的交通方式*/

		int destIndex = ui.combo_destCity->currentIndex();/*获取起始城市索引*/
		SetTransList(ui.listWidget_trans, false, sys->GetTransList(srcIndex, destIndex, Vehicle::all));/*修改时刻表*/
		});

	/*处理终点城市更换事件*/
	connect(ui.combo_destCity, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int destIndex) {
		/*TODO: 判断用户选择的交通方式*/

		int srcIndex = ui.combo_srcCity->currentIndex();/*获取起始城市索引*/
		SetTransList(ui.listWidget_trans,false, sys->GetTransList(srcIndex, destIndex, Vehicle::all));/*修改时刻表*/
		});

	/*处理查询当前正在执行的plan事件*/
	connect(ui.btn_planMy, &QPushButton::clicked, [=]() {
		SetTransList(ui.listWidget_plan,true, user->GetPlan());
		/*切换到myPlan之后当作抛弃搜索结果,也不能confirm*/
		ui.btn_planConfirm->setEnabled(false);
		});

	/*处理查询最佳plan事件*/
	connect(ui.btn_planSearch, &QPushButton::clicked, this, &MainWindow::ShowBestPlan);
	
	/*处理更新最佳plan事件*/
	connect(ui.btn_planConfirm, &QPushButton::clicked, [=]() {
		user->SetNewPlanFlag(true);
		ui.btn_planConfirm->setEnabled(false);
		});

	/*处理弹出和关闭设置页面事件*/
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

	/*处理settings的修改事件*/
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
	/*处理时钟触发状态*/
	if (hTimerId == ev->timerId()) {
		/*更新系统日期*/
		sys->SetTimeUp();
		/*更新状态栏*/
		UpdateStatusBar();

		/*更新乘客状态*/
		if (user->GetStatus() == User::status::stay
			&& user->HaveNewPlan()) {	/*如果当前没有正在执行的计划且cachePlan有计划时*/
			user->UpdatePlan(planCache);/*则开始执行缓存的计划*/
			user->SetNewPlanFlag(false);/*没有newPlan*/
		}
		user->UpdateInfo(sys->GetTime(), sys->GetDay());
		qDebug() << QString::fromStdString(user->GetStatusName());

		/*更新travel页*/
		UpdatePageTravel();

	}
}

void MainWindow::closeEvent(QCloseEvent* ev)
{
	settingsWindow->close();/*顺便把附属窗口关掉*/
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
	//connect重连
}

/*
	更新状态栏
*/
void MainWindow::UpdateStatusBar()
{

	/*设置进度控件*/
	if (User::status::on == user->GetStatus()) {
		/*计算并设置当前transport的进度 */
		currProgress->setDisabled(false);
		/*设置进度*/
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

		/*设置起始和目的城市*/
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
		/*设置当前transport的进度为0*/
		currProgress->setDisabled(false);
		currProgress->setValue(0);

		/*设置起始和目的城市*/
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
		/*设置当前transport的进度为0且禁用进度条*/
		currProgress->setDisabled(true);
		currProgress->setValue(0);

		currDestCity->setText("NONE");
		currSrcCity->setText(QString::fromStdString(
			sys->GetCityList()
			.at(user->GetCityIndex())
			.m_name)
			.toUpper());
	}
	
	/*设置日期和时间*/
	currTime->setText(QString("%1:00").arg(sys->GetTime(), 2, 10, QLatin1Char('0')));
	currDay->setText(QString("Day %1").arg(sys->GetDay()));
	currUserStatus->setText(QString(" --- status: %1 --- ").arg(QString::fromStdString(user->GetStatusName())));

}

/*更新travel页*/
void MainWindow::UpdatePageTravel()
{
	/*更新新的旅行计划起始地点的名字 START FROM 下拉框，只提供一个选项(暂时)*/
	ui.combo_srcTravel->clear();
	if (User::status::stay == user->GetStatus()) {
		/*如果用户没有正在旅行,那么下一次旅行的起始地点为用户位置*/
		qDebug() << QString::fromStdString(user->GetStatusName());
		ui.combo_srcTravel->addItem(
			QString::fromStdString(
				sys->GetCityList()
				.at(user->GetCityIndex())
				.m_name));
	} else {
		/*如果用户正在旅行,那么更新起始地点为当前旅行计划的终点城市*/
		qDebug() << user->GetPlan().size();
		int srcIndex = user->GetPlan().at(user->GetPlan().size() - 1).m_destIndex;
		City nextSrc = sys->GetCityList().at(srcIndex);
		ui.combo_srcTravel->addItem(QString::fromStdString(nextSrc.m_name));
	}

	/*由于之前搜的计划过期了，不应该再confirm，所以禁用confirm按钮*/
	ui.btn_planConfirm->setEnabled(false);

	/*将日期同步更新(算了不更了)*/
}

/*查询并显示最佳plan*/
void MainWindow::ShowBestPlan()
{
	/*计算当前行程结束后的所在城市,即plan的起始城市*/
	int srcIndex = user->GetCityIndex();			/*默认为当前城市*/
	if (User::status::stay != user->GetStatus()) {	/*如果当前正在执行计划,则选择计划的终点城市*/
		srcIndex = user->GetPlan().at(user->GetPlan().size() - 1).m_destIndex;
	}

	/*获取plan的目的城市*/
	int destIndex = ui.combo_destTravel->currentIndex();

	/*获取起始时间和日期*/
	int startTime = sys->GetTime();	/*默认为当前时间*/
	int startDay = sys->GetDay();	/*默认为当前日期*/
	if (User::status::stay != user->GetStatus()) {	/*如果当前正在执行计划,则选择计划的终点时间*/
		startTime = user->GetPlan().at(user->GetPlan().size() - 1).m_endTime;
		startDay = user->GetPlan().at(user->GetPlan().size() - 1).m_endDay;
	}

	/*获取最晚到达时间和日期*/
	int endTime = ui.spinBox_endTime->value();
	int endDay = ui.spinBox_endDay->value();

	/*-----------------------计算最佳方案-----------------------------*/
	//qDebug() << ui_settings.check_limTime->isChecked();

	planCache = sys->FindPlan(
		srcIndex, destIndex, startDay, endDay, startTime, endTime,
		ui_settings.check_repVisit->isChecked(),		/*获取用户是否考虑限时*/
		ui_settings.check_limTime->isChecked(),			/*获取用户是否允许重复访问*/
		ui_settings.check_transRisk->isChecked());		/*获取用户是否考虑交通工具的风险*/
	SetTransList(ui.listWidget_plan, true, planCache);	/*将它展现在travel页中*/

	

	/*调整confirm按钮的状态*/
	if (planCache.empty()) {/*如果获取到的计划是空的,即搜索结果为无解*/
		ui.btn_planConfirm->setEnabled(false);	/*则不允许用户确认行程*/
	} else{
		ui.btn_planConfirm->setEnabled(true);	/*允许用户确认行程*/
	}
}

void MainWindow::OnSettingChanged(int state)
{
	/*不限时也不考虑交通方式的风险的情况下，不可以选择允许重复访问*/

	/*如果第一第二个checkbox中任意一个被取消的时候,查看另一个,如果也被取消了,那就设置第三个选项为disable和false*/
	/*其他情况下,设置第三个框框为enable*/
	if(!ui_settings.check_limTime->isChecked()			
		&& !ui_settings.check_transRisk->isChecked()) {/*两个都没有打勾*/
		ui_settings.check_repVisit->setEnabled(false);	/*禁用第三个*/
		ui_settings.check_repVisit->setChecked(false);	/*取消第三个勾*/
	} else {
		ui_settings.check_repVisit->setEnabled(true);	/*其他情况下允许选择允许重复*/
	}
	
}
