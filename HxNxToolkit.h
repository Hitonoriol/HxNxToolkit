#pragma once

#include <QMainWindow>
#include "ui_HxNxToolkit.h"

#include "Enums/ToolType.h"

#include <QTimer>

class Tab;
class Component;

class HxNxToolkit : public QMainWindow
{
	Q_OBJECT

public:
	HxNxToolkit(QWidget *parent = nullptr);
	~HxNxToolkit();

	Tab* NewTab();
	Tab* GetCurrentTab();

	using Tool = ToolType;
	Q_ENUM(Tool)

public slots:
	void NewTabTriggered();
	void OnTabClose(int idx);

	void Autosave();

	void SaveTabTriggered();
	void LoadTabTriggered();

	void LoadComponent(ToolType componentType, const QJsonObject& state);

	// General
	Component* OpenBaseConverter();
	Component* OpenCalculator();

	// Productivity
	Component* OpenChecklist();
	Component* OpenTaskTracker();

	// Time
	Component* OpenStopwatch();
	Component* OpenTimer();

private:
	void SaveTab(int idx);
	void SaveTab();
	void LoadTab();

	Ui::HxNxToolkitClass ui;
	QTimer autosaveTimer;
	int curTabIdx{};
};
