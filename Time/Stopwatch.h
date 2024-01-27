#pragma once

#include <QWidget>
#include "ui_Stopwatch.h"

#include <QTimer>
#include <QElapsedTimer>

class Stopwatch : public QWidget
{
	Q_OBJECT

public:
	Stopwatch(QWidget *parent = nullptr);
	~Stopwatch();

	QString GetTimeString(int64_t timeMs);

private slots:
	void OnStartPausePress();
	void OnStopPress();
	void OnTimePointPress();

	void UpdateTimerTick();

private:
	Ui::StopwatchClass ui;

	QTimer updateTimer;
	QElapsedTimer timer;

	int64_t elapsed{};
	int64_t lastPoint{};
};
