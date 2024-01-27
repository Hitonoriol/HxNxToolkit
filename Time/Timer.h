#pragma once

#include <QWidget>
#include "ui_Timer.h"

class Timer : public QWidget
{
	Q_OBJECT

public:
	Timer(QWidget *parent = nullptr);
	~Timer();

private:
	Ui::TimerClass ui;
};
