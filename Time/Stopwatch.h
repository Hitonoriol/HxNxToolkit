#pragma once

#include <QWidget>
#include "ui_Stopwatch.h"

#include "UI/Component.h"

#include <QTimer>

class Stopwatch : public Component
{
	Q_OBJECT

public:
	Stopwatch(QWidget *parent = nullptr);
	~Stopwatch();

	virtual QJsonObject SaveState() override;
	virtual void LoadState(const QJsonObject& state) override;

private slots:
	void OnStartPausePress();
	void OnStopPress();
	void OnTimePointPress();

	void UpdateTimerTick();

private:
	int64_t Elapsed();

	Ui::StopwatchClass ui;

	QTimer updateTimer;
	int64_t startTime{};

	int64_t savedTime{};
	int64_t lastPoint{};
};
