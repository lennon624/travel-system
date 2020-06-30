#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent),
	user(new User("USERNAME", City())),
	sys(new TransSystem),
	gTime(0)
{
	/*��ʼ��ui����*/
	ui.setupUi(this);
	/*���״̬����ǩ*/
	QProgressBar* progress = new QProgressBar(this);
	progress->setValue(50);
	ui.statusBar->addWidget(new QLabel("text", this));
	ui.statusBar->addWidget(progress);
	ui.statusBar->addWidget(new QLabel("text", this));

	

	/*��ʼ������ҳ��*/
	/*��ʼ�������б�*/  /*TODO: ����combo box*/
	/*SetCityList(ui.listWidget_src, sys->GetCityList());
	SetCityList(ui.listWidget_dest, sys->GetCityList());*/
	SetCityList(ui.combo_srcCity, sys->GetCityList());
	SetCityList(ui.combo_destCity, sys->GetCityList());

	/*��ʼ�����г����б�*/
	vector<Transport> v({ Transport(),Transport() ,Transport() ,Transport() ,Transport() ,Transport()
		,Transport() ,Transport() ,Transport() ,Transport() ,Transport() });
	SetTransList(ui.listWidget_trans,v);

	/*TransportFrame* tFrame = new TransportFrame(Transport(), ui.listWidget_trans);
	QListWidgetItem* item = new QListWidgetItem(ui.listWidget_trans);
	ui.listWidget_trans->setResizeMode(QListView::ResizeMode::Adjust);
	item->setSizeHint(tFrame->size());*/
	//ui.listWidget_trans->setItemWidget(item, tFrame);
	

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
	connect(ui.combo_destCity, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
		qDebug() << QString::fromStdString(sys->GetCityList()[index].m_name);
		});
	connect(ui.combo_srcCity, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
		qDebug() << QString::fromStdString(sys->GetCityList()[index].m_name);
		ui.combo_srcCity->setDisabled(true);
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
	//connect����
}
