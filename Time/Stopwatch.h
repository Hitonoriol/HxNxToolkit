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
	void Start(bool updateState);
	void Pause(bool updateState);

	void AddTimePoint(const QString& time, const QString& diff);
	void AddTimePoint(int64_t time, int64_t diff);

	int64_t Elapsed();

	Ui::StopwatchClass ui;

	QTimer updateTimer;
	int64_t startTime{};

	int64_t savedTime{};
	int64_t lastPoint{};
};
