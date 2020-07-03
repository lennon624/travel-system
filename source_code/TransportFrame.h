#pragma once

#include <QFrame>
#include <QDebug>
#include "Transport.h"
namespace Ui { class TransportFrame; };

class TransportFrame : public QFrame
{
	Q_OBJECT

public:
	//TransportFrame(QWidget *parent = Q_NULLPTR);
	TransportFrame(const Transport& trans, const TransSystem& sys, bool viewTrueDay, QWidget* parent = Q_NULLPTR);

	~TransportFrame();

	void SetTransport(const Transport& trans, const TransSystem& sys, bool viewTrueDay, bool refresh = true);

private:
	Ui::TransportFrame *ui;
	Transport m_trans;
};
