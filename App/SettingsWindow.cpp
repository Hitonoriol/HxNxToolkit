#include "SettingsWindow.h"
#include "Util/Settings.h"

SettingsWindow::SettingsWindow(QWidget *parent)
	: QWidget(parent, Qt::Window)
{
	ui.setupUi(this);
	ui.MinimizeToTrayBox->setChecked(Settings::GetBool(Option::HideWhenMinimized));
	ui.CloseToTrayBox->setChecked(Settings::GetBool(Option::HideWhenClosed));
	ui.RestoreSessionBox->setChecked(Settings::GetBool(Option::RestorePreviousSession));
	ui.AutosaveIntervalBox->setValue(Settings::GetInt(Option::AutosaveInterval));
}

SettingsWindow::~SettingsWindow()
{}

void SettingsWindow::OnApply()
{
	Settings::Set(Option::AutosaveInterval, ui.AutosaveIntervalBox->value());
	Settings::Set(Option::HideWhenClosed, ui.CloseToTrayBox->isChecked());
	Settings::Set(Option::HideWhenMinimized, ui.MinimizeToTrayBox->isChecked());
	Settings::Set(Option::RestorePreviousSession, ui.RestoreSessionBox->isChecked());
	close();
}

void SettingsWindow::OnCancel()
{
	close();
}
