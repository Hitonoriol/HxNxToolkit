#pragma once

#include <QWidget>
#include "ui_TaskTrackerEntry.h"

class TaskTrackerEntry : public QWidget
{
	Q_OBJECT

public:
	TaskTrackerEntry(QWidget *parent = nullptr);
	~TaskTrackerEntry();

	void SetNumber(uint64_t number);
	uint64_t GetNumber();

	void SetStartTime(const QString& time);
	QString GetStartTime() const;

	void SetEndTime(const QString& time);
	QString GetEndTime() const;

	QString GetDescription() const;
	void SetDescription(const QString& description);

signals:
	void EndButtonPressed();

private slots:
	void OnEndButtonPress();

private:
	Ui::TaskTrackerEntryClass ui;
};
