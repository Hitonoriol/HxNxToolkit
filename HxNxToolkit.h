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

public slots:
	void OpenBaseConverter();
	void OpenCalculator();

private:
	Ui::HxNxToolkitClass ui;

	int curTabIdx{};
};
