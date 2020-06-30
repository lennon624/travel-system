#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent),
	user(new User("USERNAME", City())),
	sys(new TransSystem),
	gTime(0)
{
	/*初始化ui界面*/
	ui.setupUi(this);
	
	/*添加状态栏标签*/
	currProgress = new QProgressBar(this);
	currSrcCity = new QLabel(this);
	currDestCity = new QLabel(this);

	ui.statusBar->addPermanentWidget(currSrcCity);
	ui.statusBar->addPermanentWidget(currProgress);
	ui.statusBar->addPermanentWidget(currDestCity);

	

	/*debug 设置user状态*/
	user->UpdatePlan({ Transport(
		sys->GetCityList()[0],
		sys->GetCityList()[2],
		Vehicle::bus,0,10) }); /*从城市0到城市2,  0:00到10:00*/
	/*debug 把系统时间调整到3*/
	user->UpdateInfo(sys->GetTime());
	sys->SetTimeUp();
	sys->SetTimeUp();
	sys->SetTimeUp();


	/*初始化所有页面*/
	
	/*初始化状态栏标签*/
	UpdateStatusBar();

	/*初始化城市列表*/  /*TODO: 改用combo box*/
	/*SetCityList(ui.listWidget_src, sys->GetCityList());
	SetCityList(ui.listWidget_dest, sys->GetCityList());*/
	SetCityList(ui.combo_srcCity, sys->GetCityList());
	SetCityList(ui.combo_destCity, sys->GetCityList());
	SetCityList(ui.combo_destTravel, sys->GetCityList());
	

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
	
	/*处理起始城市更换事件*/
	connect(ui.combo_srcCity, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int srcIndex) {
		qDebug() << QString::fromStdString(sys->GetCityList()[srcIndex].m_name);

		/*TODO: 判断用户选择的交通方式*/
		
		int destIndex = ui.combo_destCity->currentIndex();/*获取起始城市索引*/
		SetTransList(ui.listWidget_trans, sys->GetTransList(srcIndex, destIndex, Vehicle::all));/*修改时刻表*/
		});

	/*处理终点城市更换事件*/
	connect(ui.combo_destCity, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int destIndex) {
		qDebug() << QString::fromStdString(sys->GetCityList()[destIndex].m_name);

		/*TODO: 判断用户选择的交通方式*/

		int srcIndex = ui.combo_srcCity->currentIndex();/*获取起始城市索引*/
		SetTransList(ui.listWidget_trans, sys->GetTransList(srcIndex, destIndex, Vehicle::all));/*修改时刻表*/
		});

}

MainWindow::~MainWindow() {
	delete sys;
	delete user;
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

void MainWindow::UpdateStatusBar()
{
	
	if (User::status::on == user->GetStatus()) {
		/*计算并设置当前transport的进度 */
		currProgress->setDisabled(false);
		/*设置进度*/
		int totalTime = TransSystem::CountTime(
			user->GetTransport().m_startTime,
			user->GetTransport().m_endTime);
		int pastTime = TransSystem::CountTime(
			user->GetTransport().m_startTime,
			sys->GetTime()
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
	
	//switch (user->GetStatus())
	//{

	//case User::status::on:		/*user正在上路*/

	//	
	//	break;


	//case User::status::suspend:	/*user正在中转*/

	//	
	//	break;


	//case User::status::stay:	/*user没有计划*/
	//	
	//	break;
	//default:
	//	break;
	//}

}
