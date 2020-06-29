#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	TransportFrame* tFrame = new TransportFrame(Transport(), ui.listWidget_trans);
   /* QListWidget* list = new QListWidget(ui.centralWidget);
    QListWidgetItem* item1 = new QListWidgetItem(list);
    list->setItemWidget(item1, tFrame);*/
    QListWidgetItem* item = new QListWidgetItem(ui.listWidget_trans);
    ui.listWidget_trans->setResizeMode(QListView::ResizeMode::Adjust);
    item->setSizeHint(tFrame->size());
    ui.listWidget_trans->setItemWidget(item, tFrame);

}
