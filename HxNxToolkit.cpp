#include "HxNxToolkit.h"

#include "General/BaseConverter.h"
#include "General/Calculator.h"

#include "Productivity/Checklist/Checklist.h"
#include "Productivity/TaskTracker/TaskTracker.h"

#include "Time/Stopwatch.h"
#include "Time/Timer.h"

#include "UI/Tab.h"

#include <QDateTime>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QFileDialog>
#include <QByteArray>

HxNxToolkit::HxNxToolkit(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	NewTab();
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

Component* HxNxToolkit::OpenTaskTracker()
{
	auto taskTracker = new TaskTracker;
	GetCurrentTab()->AddComponent(taskTracker, "Task tracker");
	return taskTracker;
}

void HxNxToolkit::SaveTab()
{
	auto currentTab = GetCurrentTab();
	auto tabJson = currentTab->SaveState();
	tabJson["Title"] = ui.Tabs->tabText(ui.Tabs->currentIndex());

	QJsonDocument doc(tabJson);
	auto savePath = currentTab->GetSavePath();
	QFile saveFile;
	if (savePath.isEmpty()) {
		QFileDialog dialog(this);
		dialog.setFileMode(QFileDialog::AnyFile);
		dialog.setNameFilter("HxNx Tab File (*.hxnx-tab)");
		dialog.setAcceptMode(QFileDialog::AcceptSave);

		QStringList files;
		while (files.isEmpty()) {
			dialog.exec();
			files = dialog.selectedFiles();
		}

		auto newPath = files.first();
		saveFile.setFileName(newPath);
		currentTab->SetSavePath(newPath);
	} else {
		saveFile.setFileName(savePath);
	}
	
	if (!saveFile.open(QFile::WriteOnly)) {
		return;
	}

	saveFile.write(doc.toJson());
	saveFile.close();
}

void HxNxToolkit::LoadTab()
{
	auto tab = NewTab();

	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter("HxNx Tab File (*.hxnx-tab)");
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	
	QStringList files;
	while (files.isEmpty()) {
		dialog.exec();
		files = dialog.selectedFiles();
	}

	auto tabPath = files.first();
	QFile tabFile(tabPath);

	if (!tabFile.open(QFile::ReadOnly)) {
		return;
	}

	auto tabDoc = QJsonDocument::fromJson(tabFile.readAll());
	auto tabObject = tabDoc.object();
	ui.Tabs->setTabText(ui.Tabs->currentIndex(), tabObject["Title"].toString());
	tab->LoadState(tabObject);
}
