#pragma once

#include <QWidget>
#include "ui_Tab.h"

class Tab : public QWidget
{
	Q_OBJECT

public:
	Tab(QWidget* parent = nullptr);
	~Tab();

	void AddWidget(QWidget* parent, const QString& title = "Tool");

private:
	Ui::TabClass ui;
};
