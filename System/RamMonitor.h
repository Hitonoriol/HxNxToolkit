#pragma once

#include <QWidget>
#include "ui_RamMonitor.h"

#include "UI/Component.h"
#include <QTimer>

class RamMonitor : public Component
{
	Q_OBJECT

public:
	RamMonitor(QWidget *parent = nullptr);
	~RamMonitor();

private slots:
	void Update();

private:
	Ui::RamMonitorClass ui;
	QTimer updateTimer;
};
