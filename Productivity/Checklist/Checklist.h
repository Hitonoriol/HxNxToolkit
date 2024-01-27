#pragma once

#include <QWidget>
#include "ui_Checklist.h"

class Checklist : public QWidget
{
	Q_OBJECT

public:
	Checklist(QWidget *parent = nullptr);
	~Checklist();

public slots:
	void AddEntryTriggered();

private:
	Ui::ChecklistClass ui;
};
