#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent),
	user(new User("USERNAME", City())),
	sys(new TransSystem)
	/*gTime(0),
	gDay(0)*/
{
	/*初始化ui界面*/
	ui.setupUi(this);
	
	/*添加状态栏标签*/
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

	/*debug 设置user状态*/
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
	); /*从城市0到城市2,  0:00到10:00*/
	/*debug 把系统时间调整到3*/
	user->UpdateInfo(sys->GetTime(),sys->GetDay());


	/*初始化所有页面*/
	
	/*初始化状态栏标签*/
	UpdateStatusBar();

	/*初始化城市列表*/  /*TODO: 改用combo box*/
	/*SetCityList(ui.listWidget_src, sys->GetCityList());
	SetCityList(ui.listWidget_dest, sys->GetCityList());*/
	SetCityList(ui.combo_srcCity, sys->GetCityList());
	SetCityList(ui.combo_destCity, sys->GetCityList());
	SetCityList(ui.combo_destTravel, sys->GetCityList());
	SetCityList(ui.combo_srcTravel, { user->GetCity() });
	

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
		qDebug() << QString::fromStdString(sys->GetCityList().at(srcIndex).m_name);

		/*TODO: 判断用户选择的交通方式*/

		int destIndex = ui.combo_destCity->currentIndex();/*获取起始城市索引*/
		SetTransList(ui.listWidget_trans, sys->GetTransList(srcIndex, destIndex, Vehicle::all));/*修改时刻表*/
		});
	/*处理终点城市更换事件*/
	connect(ui.combo_destCity, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int destIndex) {
		qDebug() << QString::fromStdString(sys->GetCityList().at(destIndex).m_name);

		/*TODO: 判断用户选择的交通方式*/

		int srcIndex = ui.combo_srcCity->currentIndex();/*获取起始城市索引*/
		SetTransList(ui.listWidget_trans, sys->GetTransList(srcIndex, destIndex, Vehicle::all));/*修改时刻表*/
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
			QString::fromStdString(user->GetTransport().m_srcCity.m_name)
		);
		currDestCity->setText(
			QString::fromStdString(user->GetTransport().m_destCity.m_name)
		);
	} else if (User::status::suspend == user->GetStatus()){
		/*设置当前transport的进度为0*/
		currProgress->setDisabled(false);
		currProgress->setValue(0);

		/*设置起始和目的城市*/
		currSrcCity->setText(
			QString::fromStdString(user->GetTransport().m_srcCity.m_name)
		);
		currDestCity->setText(
			QString::fromStdString(user->GetTransport().m_destCity.m_name)
		);
	} else if (User::status::stay == user->GetStatus()) {
		/*设置当前transport的进度为0且禁用进度条*/
		currProgress->setDisabled(true);
		currProgress->setValue(0);

		currDestCity->setText("NONE");
		currSrcCity->setText(QString::fromStdString(user->GetCity().m_name).toUpper());
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
		ui.combo_srcTravel->addItem(QString::fromStdString(user->GetCity().m_name));
	} else {
		/*如果用户正在旅行,那么更新起始地点为当前旅行计划的终点城市*/
		qDebug() << user->GetPlan().size();
		City nextSrc = user->GetPlan().at(user->GetPlan().size() - 1).m_destCity;
		ui.combo_srcTravel->addItem(QString::fromStdString(nextSrc.m_name));
	}

	/*将日期同步更新(算了不更了)*/
}
