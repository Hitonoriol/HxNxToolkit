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

	QAction* AddComponentMenuAction(const QString& categoryName, const QString& componentName);

	using Tool = ToolType;
	Q_ENUM(Tool)

public slots:
	void NewTabTriggered();
	void OnTabClose(int idx);
	void OnTabModified(Tab* tab);
	void OnTabSaved(Tab* tab);

	void Autosave();

	void SaveTabTriggered();
	void LoadTabTriggered();

	void LoadComponent(ToolType componentType, const QJsonObject& state);

private:
	void SaveTab(int idx);
	void SaveTab();
	void LoadTab();

	Ui::HxNxToolkitClass ui;
	QTimer autosaveTimer;
	int curTabIdx{};
};
