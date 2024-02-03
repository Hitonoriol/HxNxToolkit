#include "HxNxToolkit.h"

#include "General/BaseConverter.h"
#include "General/Calculator.h"

#include "Productivity/Checklist/Checklist.h"
#include "Productivity/TaskTracker/TaskTracker.h"

#include "Time/Stopwatch.h"
#include "Time/Timer.h"

#include "Random/RandomNumber.h"

#include "UI/Tab.h"
#include "Util/Settings.h"

#include <QDateTime>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QByteArray>
#include <QRegularExpression>

HxNxToolkit::HxNxToolkit(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	NewTab();

	auto lastSaveDir = Settings::GetString(Settings::LastSaveDir);
	auto savePath = lastSaveDir.isEmpty() ? QApplication::applicationDirPath() + "/Workspace" : lastSaveDir;
	QDir().mkdir(savePath);
	Settings::SetString(Settings::LastSaveDir, savePath);

	connect(&autosaveTimer, &QTimer::timeout, this, &HxNxToolkit::Autosave);
	autosaveTimer.start(std::chrono::milliseconds{60'000});
}

HxNxToolkit::~HxNxToolkit()
{}

void HxNxToolkit::LoadComponent(ToolType componentType, const QJsonObject& state)
{
	Component* component{};
	switch (componentType) {
	case ToolType::BaseConverter:
		component = OpenBaseConverter();
		break;

	case ToolType::Calculator:
		component = OpenCalculator();
		break;

	case ToolType::Checklist:
		component = OpenChecklist();
		break;

	case ToolType::TaskTracker:
		component = OpenTaskTracker();
		break;

	case ToolType::Stopwatch:
		component = OpenStopwatch();
		break;

	case ToolType::Timer:
		component = OpenTimer();
		break;

	case ToolType::RandomNumber:
		component = OpenRandomNumber();
		break;

	default:
		break;
	}

	if (component) {
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

void HxNxToolkit::NewTabTriggered()
{
	NewTab();
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

Component* HxNxToolkit::OpenBaseConverter()
{
	auto converter = new BaseConverter;
	GetCurrentTab()->AddComponent(converter, "Base converter");
	return converter;
}

Component* HxNxToolkit::OpenCalculator()
{
	auto calculator = new Calculator;
	GetCurrentTab()->AddComponent(calculator, "Calculator");
	return calculator;
}

Component* HxNxToolkit::OpenChecklist()
{
	auto checklist = new Checklist;
	GetCurrentTab()->AddComponent(checklist, "Checklist");
	return checklist;
}

Component* HxNxToolkit::OpenStopwatch()
{
	auto stopwatch = new Stopwatch;
	GetCurrentTab()->AddComponent(stopwatch, "Stopwatch");
	return stopwatch;
}

Component* HxNxToolkit::OpenTimer()
{
	auto timer = new Timer;
	connect(timer, &Timer::TimerCompleted, this, [this](int64_t startTime, Time duration) {
		setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
		activateWindow();

		auto durationStr = Time::GetTimeString(duration.GetTimeMs());
		auto startTimeStr = Time::GetTimeString(QDateTime::fromMSecsSinceEpoch(startTime));
		auto notification = new QMessageBox(this);
		notification->setWindowTitle("Timer");
		notification->setText("Timer has completed!");
		notification->setInformativeText(QString("Duration: %1\nStarted at: %2").arg(durationStr).arg(startTimeStr));
		notification->show();
	});

	GetCurrentTab()->AddComponent(timer, "Timer");
	return timer;
}

Component* HxNxToolkit::OpenRandomNumber()
{
	auto rndNumber = new RandomNumber;
	GetCurrentTab()->AddComponent(rndNumber, "Random number");
	return rndNumber;
}

Component* HxNxToolkit::OpenTaskTracker()
{
	auto taskTracker = new TaskTracker;
	GetCurrentTab()->AddComponent(taskTracker, "Task tracker");
	return taskTracker;
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
}
