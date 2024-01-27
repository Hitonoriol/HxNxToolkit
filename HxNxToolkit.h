#pragma once

#include <QMainWindow>
#include "ui_HxNxToolkit.h"

#include "UI/Tab.h"

class HxNxToolkit : public QMainWindow
{
	Q_OBJECT

public:
	HxNxToolkit(QWidget *parent = nullptr);
	~HxNxToolkit();

	Tab* NewTab();
	Tab* GetCurrentTab();

	enum class Tool
	{
		// General
		BaseConverter,
		Calculator,

		// Productivity
		Checklist,

		// Time
		Stopwatch
	};
	Q_ENUM(Tool)

public slots:
	void NewTabTriggered();

	void OpenBaseConverter();
	void OpenCalculator();

	void OpenChecklist();

	void OpenStopwatch();

private:
	Ui::HxNxToolkitClass ui;

	int curTabIdx{};
};
