#pragma once

#include <QWidget>
#include "ui_Timer.h"

#include "Util/Time.h"
#include "UI/Component.h"

#include <QTimer>

class Timer : public Component
{
	Q_OBJECT

public:
	Timer(QWidget *parent = nullptr);
	~Timer();

	virtual QJsonObject SaveState() override;
	virtual void LoadState(const QJsonObject& state) override;

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
