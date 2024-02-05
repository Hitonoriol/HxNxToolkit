#include "HxNxToolkit.h"

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
	NewTab();

	auto lastSaveDir = Settings::GetString(Settings::LastSaveDir);
	auto savePath = lastSaveDir.isEmpty() ? QApplication::applicationDirPath() + "/Workspace" : lastSaveDir;
	QDir().mkdir(savePath);
	Settings::SetString(Settings::LastSaveDir, savePath);

	connect(&autosaveTimer, &QTimer::timeout, this, &HxNxToolkit::Autosave);
	autosaveTimer.start(std::chrono::milliseconds{60'000});

	trayMenu = new QMenu(this);
	auto closeAction = new QAction("&Close", this);
	connect(closeAction, &QAction::triggered, qApp, &QCoreApplication::quit);
	trayMenu->addAction(closeAction);

	trayIcon = new QSystemTrayIcon(QIcon(":/icons/icon-tray.ico"), this);
	trayIcon->show();
	trayIcon->setContextMenu(trayMenu);
	connect(trayIcon, &QSystemTrayIcon::activated, this, &HxNxToolkit::IconActivated);
}

HxNxToolkit::~HxNxToolkit()
{}

void HxNxToolkit::IconActivated(QSystemTrayIcon::ActivationReason reason)
{
	bool visible = this->isVisible();
	if (reason == QSystemTrayIcon::ActivationReason::DoubleClick) {
		this->setVisible(!visible);
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
	if (trayIcon->isVisible()) {
		hide();
		event->ignore();
	}
}

void HxNxToolkit::changeEvent(QEvent* event)
{
	switch (event->type()) {
	case QEvent::WindowStateChange:
		if (isMinimized()) {
			hide();
		}
		break;
	default:
		break;
	}
}

void HxNxToolkit::OnTabClose(int idx)
{
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

void HxNxToolkit::SaveTab(int idx)
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
		dialog.setDirectory({Settings::GetString(Settings::LastSaveDir)});
		dialog.selectFile(suggestedName);
		dialog.setFileMode(QFileDialog::AnyFile);
		dialog.setNameFilter("HxNx Tab File (*.hxnx-tab)");
		dialog.setAcceptMode(QFileDialog::AcceptSave);

		auto result = dialog.exec();
		auto files = dialog.selectedFiles();

		auto newPath = files.isEmpty() ? QString("") : files.first();
		if (!result || newPath.isEmpty()) {
			return;
		}

		savePath = newPath;
		saveFile.setFileName(newPath);
		tab->SetSavePath(newPath);
	} else {
		saveFile.setFileName(savePath);
	}

	if (!saveFile.open(QFile::WriteOnly)) {
		tab->SetSavePath("");
		return;
	}

	saveFile.write(doc.toJson());
	Settings::SetString(Settings::LastSaveDir, savePath);
	saveFile.close();
}

void HxNxToolkit::SaveTab()
{
	auto currentTab = ui.Tabs->currentIndex();
	SaveTab(currentTab);
}

void HxNxToolkit::LoadTab()
{
	auto tab = NewTab();

	QFileDialog dialog(this);
	dialog.setDirectory({Settings::GetString(Settings::LastSaveDir)});
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter("HxNx Tab File (*.hxnx-tab)");
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	
	auto result = dialog.exec();
	auto files = dialog.selectedFiles();

	auto tabPath = files.isEmpty() ? QString("") : files.first();
	if (!result || tabPath.isEmpty()) {
		return;
	}

	QFile tabFile(tabPath);
	if (!tabFile.open(QFile::ReadOnly)) {
		return;
	}

	auto tabDoc = QJsonDocument::fromJson(tabFile.readAll());
	auto tabObject = tabDoc.object();
	ui.Tabs->setTabText(ui.Tabs->currentIndex(), tabObject["Title"].toString());
	tab->LoadState(tabObject);
	tab->SetSavePath(tabPath);
}
