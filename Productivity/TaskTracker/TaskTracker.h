#pragma once

#include <QWidget>
#include "ui_TaskTracker.h"

#include "UI/Component.h"

class TaskTrackerEntry;

class TaskTracker : public Component
{
	Q_OBJECT

public:
	TaskTracker(QWidget *parent = nullptr);
	~TaskTracker();

	virtual QJsonObject SaveState() override;
	virtual void LoadState(const QJsonObject& state) override;

private slots:
	void OnStartSessionPress();

private:
	TaskTrackerEntry* AddTaskEntry();
	void UpdateTotalTime();

	Ui::TaskTrackerClass ui;

	TaskTrackerEntry* lastEntry{};
};
