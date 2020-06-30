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
	currProgress = new QProgressBar(this);
	currSrcCity = new QLabel(this);
	currDestCity = new QLabel(this);

	ui.statusBar->addPermanentWidget(currSrcCity);
	ui.statusBar->addPermanentWidget(currProgress);
	ui.statusBar->addPermanentWidget(currDestCity);

	

	/*��ʼ������ҳ��*/
	
	/*��ʼ��״̬����ǩ*/
	UpdateStatusBar();

	/*��ʼ�������б�*/  /*TODO: ����combo box*/
	/*SetCityList(ui.listWidget_src, sys->GetCityList());
	SetCityList(ui.listWidget_dest, sys->GetCityList());*/
	SetCityList(ui.combo_srcCity, sys->GetCityList());
	SetCityList(ui.combo_destCity, sys->GetCityList());
	SetCityList(ui.combo_destTravel, sys->GetCityList());
	

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
	
	/*������ʼ���и����¼�*/
	connect(ui.combo_srcCity, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int srcIndex) {
		qDebug() << QString::fromStdString(sys->GetCityList()[srcIndex].m_name);

		/*TODO: �ж��û�ѡ��Ľ�ͨ��ʽ*/
		
		int destIndex = ui.combo_destCity->currentIndex();/*��ȡ��ʼ��������*/
		SetTransList(ui.listWidget_trans, sys->GetTransList(srcIndex, destIndex, Vehicle::all));/*�޸�ʱ�̱�*/
		});

	/*�����յ���и����¼�*/
	connect(ui.combo_destCity, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int destIndex) {
		qDebug() << QString::fromStdString(sys->GetCityList()[destIndex].m_name);

		/*TODO: �ж��û�ѡ��Ľ�ͨ��ʽ*/

		int srcIndex = ui.combo_srcCity->currentIndex();/*��ȡ��ʼ��������*/
		SetTransList(ui.listWidget_trans, sys->GetTransList(srcIndex, destIndex, Vehicle::all));/*�޸�ʱ�̱�*/
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

void MainWindow::UpdateStatusBar()
{
	if (User::status::stay == user->GetStatus()) {
		currProgress->setDisabled(true);
		currProgress->setValue(20);
		currDestCity->setText("NONE");
		currSrcCity->setText(QString::fromStdString(user->GetCity().m_name).toUpper());
		
	}
}
