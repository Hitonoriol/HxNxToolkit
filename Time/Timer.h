#pragma once

#include <QWidget>
#include "ui_Timer.h"

#include <QTimer>
#include "Util/Time.h"

class Timer : public QWidget
{
	Q_OBJECT

public:
	Timer(QWidget *parent = nullptr);
	~Timer();

signals:
	void TimerCompleted(int64_t startTime, Time duration);

private slots:
	void OnStartButtonPress();
	void OnStopButtonPress();

	void OnUpdateTimerTick();

private:
	Ui::TimerClass ui;

	Time duration;
	int64_t startTime{};
	int64_t savedTime{};
	QTimer updateTimer;
};
