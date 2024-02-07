#include "HxNxToolkit.h"

#include "App/SettingsWindow.h"
#include "UI/Tab.h"
#include "UI/ComponentFactory.h"
#include "Util/Settings.h"
#include "Util/Time.h"

#include <QDateTime>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QByteArray>
#include <QRegularExpression>
#include <QCloseEvent>

HxNxToolkit::HxNxToolkit(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ComponentFactory::Register(this);
	CreateDefaultSettings();
	auto defaultTab = NewTab();

	connect(&autosaveTimer, &QTimer::timeout, this, &HxNxToolkit::Autosave);
	autosaveTimer.start(std::chrono::milliseconds{Settings::GetInt(Option::AutosaveInterval) * 1'000});

	trayMenu = new QMenu(this);
	auto closeAction = new QAction("&Quit", this);
	connect(closeAction, &QAction::triggered, this, &HxNxToolkit::Quit);
	connect(ui.ActionQuit, &QAction::triggered, this, &HxNxToolkit::Quit);
	trayMenu->addAction(closeAction);

	QIcon icon(":/icons/icon-app.ico");
	setWindowIcon(icon);
	trayIcon = new QSystemTrayIcon(icon, this);
	trayIcon->show();
	trayIcon->setContextMenu(trayMenu);
	connect(trayIcon, &QSystemTrayIcon::activated, this, &HxNxToolkit::IconActivated);

	ui.ActionAlwaysOnTop->setChecked(Settings::GetBool(Option::AlwaysOnTop));

	if (Settings::GetBool(Option::RestorePreviousSession)) {
		auto path = Settings::GetString(Option::LastSavedTabPath);
		LoadTab(defaultTab, path);
	}
}

HxNxToolkit::~HxNxToolkit()
{}

void HxNxToolkit::IconActivated(QSystemTrayIcon::ActivationReason reason)
{
	bool visible = this->isVisible();
	if (reason == QSystemTrayIcon::ActivationReason::DoubleClick) {
		this->setVisible(!visible);
		setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
		activateWindow();
	}
}

void HxNxToolkit::LoadComponent(ToolType componentType, const QJsonObject& state)
{
	if (auto component = ComponentFactory::CreateComponent(this, componentType)) {
		component->LoadState(state);
	}
}

Tab* HxNxToolkit::NewTab()
{
	auto tab = new Tab;
	auto title = Time::GetTimeString(QDateTime::currentDateTime());
	curTabIdx = ui.Tabs->addTab(tab, title);
	ui.Tabs->setCurrentWidget(tab);
	connect(tab, &Tab::LoadComponent, this, &HxNxToolkit::LoadComponent);
	connect(tab, &Tab::TabModified, this, &HxNxToolkit::OnTabModified);
	connect(tab, &Tab::TabSaved, this, &HxNxToolkit::OnTabSaved);
	return tab;
}

Tab* HxNxToolkit::GetCurrentTab()
{
	return dynamic_cast<Tab*>(ui.Tabs->currentWidget());
}

QAction* HxNxToolkit::AddComponentMenuAction(const QString& categoryName, const QString& componentName)
{
	auto menus = ui.MenuBar->findChildren<QMenu*>();
	auto categoryIt = std::find_if(menus.begin(), menus.end(), [=](QMenu* menu) {
		return menu->title() == categoryName;
	});

	QMenu* category{};
	if (categoryIt == menus.end()) {
		category = new QMenu(categoryName, ui.MenuBar);
		category->setObjectName(categoryName);
		ui.MenuBar->addMenu(category);
	} else {
		category = *categoryIt;
	}

	auto componentAction = new QAction(componentName, category);
	category->addAction(componentAction);
	return componentAction;
}

void HxNxToolkit::NewTabTriggered()
{
	NewTab();
}

void HxNxToolkit::closeEvent(QCloseEvent* event)
{
	if (Settings::GetBool(Option::HideWhenClosed) && trayIcon->isVisible()) {
		hide();
		event->ignore();
	}
}

void HxNxToolkit::changeEvent(QEvent* event)
{
	switch (event->type()) {
	case QEvent::WindowStateChange:
		if (Settings::GetBool(Option::HideWhenMinimized) && isMinimized()) {
			hide();
		}
		break;
	default:
		break;
	}
}

void HxNxToolkit::OnTabClose(int idx)
{
	auto tab = dynamic_cast<Tab*>(ui.Tabs->widget(idx));
	if (tab && tab->IsModified()) {
		auto result = QMessageBox::question(
			this, "Closing \"" + ui.Tabs->tabText(idx) + "\"",
			"This tab has unsaved changes. Save now?",
			QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No | QMessageBox::StandardButton::Cancel
		);

		switch (result) {
		case QMessageBox::StandardButton::Cancel:
			return;

		case QMessageBox::StandardButton::Yes:
			if (!SaveTab(idx)) {
				return;
			}
			break;

		default:
			break;
		} 
	}

	ui.Tabs->removeTab(idx);
}

void HxNxToolkit::OnTabModified(Tab* tab)
{
	auto idx = ui.Tabs->indexOf(tab);
	if (!tab->IsModified()) {
		ui.Tabs->setTabText(idx, ui.Tabs->tabText(idx) + "*");
	}
}

void HxNxToolkit::OnTabSaved(Tab* tab)
{
	auto idx = ui.Tabs->indexOf(tab);
	if (tab->IsModified()) {
		ui.Tabs->setTabText(idx, ui.Tabs->tabText(idx).removeLast());
	}
}

void HxNxToolkit::Autosave()
{
	for (size_t i = 0; i < ui.Tabs->count(); ++i) {
		auto tab = dynamic_cast<Tab*>(ui.Tabs->widget(i));
		if (!tab->GetSavePath().isEmpty() && tab->IsModified()) {
			SaveTab(i);
		}
	}
}

void HxNxToolkit::SaveTabTriggered()
{
	SaveTab();
}

void HxNxToolkit::LoadTabTriggered()
{
	LoadTab();
}

void HxNxToolkit::CloseTabTriggered()
{
	ui.Tabs->removeTab(ui.Tabs->currentIndex());
}

void HxNxToolkit::AlwaysOnTopToggled(bool onTop)
{
	auto flags = windowFlags();
	if (onTop) {
		flags |= Qt::WindowStaysOnTopHint;
	} else {
		flags &= ~Qt::WindowStaysOnTopHint;
	}

	setWindowFlags(flags);
	show();

	Settings::Set(Option::AlwaysOnTop, onTop);
}

void HxNxToolkit::SettingsTriggered()
{
	auto settings = new SettingsWindow(this);
	
	auto flags = settings->windowFlags();
	flags &= ~Qt::WindowMinimizeButtonHint;
	flags &= ~Qt::WindowMaximizeButtonHint;
	flags &= ~Qt::WindowCloseButtonHint;
	settings->setWindowFlags(flags);

	settings->setWindowModality(Qt::ApplicationModal);
	settings->show();
}

void HxNxToolkit::CreateDefaultSettings()
{
	Settings::SetDefault(Option::AutosaveInterval, 60);
	Settings::SetDefault(Option::AlwaysOnTop, false);
	Settings::SetDefault(Option::HideWhenClosed, false);
	Settings::SetDefault(Option::HideWhenMinimized, false);
	Settings::SetDefault(Option::RestorePreviousSession, false);

	// Default save path
	auto lastSaveDir = Settings::GetString(Option::LastSaveDir);
	auto savePath = lastSaveDir.isEmpty() ? QApplication::applicationDirPath() + "/Workspace" : lastSaveDir;
	QDir().mkdir(savePath);
	Settings::Set(Option::LastSaveDir, savePath);
}

bool HxNxToolkit::SaveTab(int idx)
{
	auto tab = dynamic_cast<Tab*>(ui.Tabs->widget(idx));
	auto tabJson = tab->SaveState();
	auto title = ui.Tabs->tabText(idx);
	tabJson["Title"] = title;

	QJsonDocument doc(tabJson);
	auto savePath = tab->GetSavePath();
	QFile saveFile;
	if (savePath.isEmpty()) {
		auto suggestedName = title;
		suggestedName.replace(QRegularExpression("[^a-zA-Z0-9]"), "-");
		suggestedName.replace("--", "-");

		QFileDialog dialog(this);
		dialog.setDirectory({Settings::GetString(Option::LastSaveDir)});
		dialog.selectFile(suggestedName);
		dialog.setFileMode(QFileDialog::AnyFile);
		dialog.setNameFilter("HxNx Tab File (*.hxnx-tab)");
		dialog.setAcceptMode(QFileDialog::AcceptSave);

		auto result = dialog.exec();
		auto files = dialog.selectedFiles();

		auto newPath = files.isEmpty() ? QString("") : files.first();
		if (!result || newPath.isEmpty()) {
			return false;
		}

		savePath = newPath;
		saveFile.setFileName(newPath);
		tab->SetSavePath(newPath);
	} else {
		saveFile.setFileName(savePath);
	}

	if (!saveFile.open(QFile::WriteOnly)) {
		tab->SetSavePath("");
		return false;
	}

	saveFile.write(doc.toJson());
	Settings::Set(Option::LastSaveDir, QFileInfo(savePath).absolutePath());
	Settings::Set(Option::LastSavedTabPath, savePath);
	saveFile.close();
	return true;
}

bool HxNxToolkit::SaveTab()
{
	auto currentTab = ui.Tabs->currentIndex();
	return SaveTab(currentTab);
}

void HxNxToolkit::LoadTab()
{
	auto tab = NewTab();

	QFileDialog dialog(this);
	dialog.setDirectory({Settings::GetString(Option::LastSaveDir)});
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter("HxNx Tab File (*.hxnx-tab)");
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	
	auto result = dialog.exec();
	auto files = dialog.selectedFiles();

	auto tabPath = files.isEmpty() ? QString("") : files.first();
	if (!result || tabPath.isEmpty()) {
		return;
	}

	LoadTab(tab, tabPath);
}

void HxNxToolkit::LoadTab(Tab* tab, const QString& tabPath)
{
	QFile tabFile(tabPath);
	if (!tabFile.open(QFile::ReadOnly)) {
		return;
	}

	auto tabDoc = QJsonDocument::fromJson(tabFile.readAll());
	auto tabObject = tabDoc.object();
	ui.Tabs->setTabText(ui.Tabs->currentIndex(), tabObject["Title"].toString());
	tab->LoadState(tabObject);
	tab->SetSavePath(tabPath);
	Settings::Set(Option::LastSavedTabPath, tabPath);
}

void HxNxToolkit::Quit()
{
	for (size_t i = 0; i < ui.Tabs->count(); ++i) {
		OnTabClose(i);
	}

	QApplication::quit();
}
