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
		Stopwatch,
		Timer
	};
	Q_ENUM(Tool)

public slots:
	void NewTabTriggered();

	// General
	void OpenBaseConverter();
	void OpenCalculator();

	// Productivity
	void OpenChecklist();

	// Time
	void OpenStopwatch();
	void OpenTimer();

private:
	Ui::HxNxToolkitClass ui;

	int curTabIdx{};
};
