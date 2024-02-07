#pragma once

#include <QWidget>
#include "ui_SettingsWindow.h"

class SettingsWindow : public QWidget
{
	Q_OBJECT

public:
	SettingsWindow(QWidget *parent = nullptr);
	~SettingsWindow();

private slots:
	void OnApply();
	void OnCancel();

private:
	Ui::SettingsWindowClass ui;
};
