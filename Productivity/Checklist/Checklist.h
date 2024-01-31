#pragma once

#include <QWidget>
#include "ui_Checklist.h"

#include "UI/Component.h"

class ChecklistEntry;

class Checklist : public Component
{
	Q_OBJECT

public:
	Checklist(QWidget *parent = nullptr);
	~Checklist();

	virtual QJsonObject SaveState() override;
	virtual void LoadState(const QJsonObject& state) override;

public slots:
	void AddEntryTriggered();

private:
	ChecklistEntry* AddEntry();

	Ui::ChecklistClass ui;
};
