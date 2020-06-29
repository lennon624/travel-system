#pragma once

#include <QFrame>
#include "Transport.h"
namespace Ui { class TransportFrame; };

class TransportFrame : public QFrame
{
	Q_OBJECT

public:
	//TransportFrame(QWidget *parent = Q_NULLPTR);
	TransportFrame(const Transport& trans, QWidget* parent = Q_NULLPTR);

	~TransportFrame();

	void SetTransport(const Transport& trans, bool refresh = true);

private:
	Ui::TransportFrame *ui;
	Transport m_trans;
};
