#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent),
	user(new User("USERNAME", City())),
	sys(new TransSystem),
	gTime(0)
{
	ui.setupUi(this);


	

	/*初始化所有页面*/
	/*初始化城市列表*/
	SetCityList(ui.listWidget_src, sys->GetCityList());
	SetCityList(ui.listWidget_dest, sys->GetCityList());

	/*初始化所有车次列表*/
	vector<Transport> v({ Transport(),Transport() ,Transport() ,Transport() ,Transport() ,Transport() });
	SetTransList(ui.listWidget_trans,v);

	/*TransportFrame* tFrame = new TransportFrame(Transport(), ui.listWidget_trans);
	QListWidgetItem* item = new QListWidgetItem(ui.listWidget_trans);
	ui.listWidget_trans->setResizeMode(QListView::ResizeMode::Adjust);
	item->setSizeHint(tFrame->size());*/
	//ui.listWidget_trans->setItemWidget(item, tFrame);
	

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
	connect(ui.listWidget_dest, &QListWidget::itemSelectionChanged, [=]() {
		qDebug() << QString::fromStdString(sys->GetCityList()[ui.listWidget_dest->currentRow()].m_name);
		//更新这两个城市之间的车次 setTransList
		
		});
	connect(ui.listWidget_src, &QListWidget::itemSelectionChanged, [=]() {
		qDebug() << QString::fromStdString(sys->GetCityList()[ui.listWidget_src->currentRow()].m_name);
		//更新这两个城市之间的车次 setTransList
		});
	
}

MainWindow::~MainWindow() {
	delete sys;
	delete user;
}

void MainWindow::SetCityList(QListWidget* listWidget, const vector<City>& listCity)
{
	for (const City& city : listCity) {
		listWidget->addItem(QString::fromStdString(city.m_name));
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
