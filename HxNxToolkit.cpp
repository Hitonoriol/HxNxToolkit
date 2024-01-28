#include "HxNxToolkit.h"

#include "General/BaseConverter.h"
#include "General/Calculator.h"

#include "Productivity/Checklist/Checklist.h"

#include "Time/Stopwatch.h"
#include "Time/Timer.h"

#include <QDateTime>
#include <QMessageBox>

HxNxToolkit::HxNxToolkit(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	NewTab();
}

HxNxToolkit::~HxNxToolkit()
{}

Tab* HxNxToolkit::NewTab()
{
	auto tab = new Tab;
	auto title = Time::GetTimeString(QDateTime::currentDateTime());
	curTabIdx = ui.Tabs->addTab(tab, title);
	ui.Tabs->setCurrentWidget(tab);
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

void HxNxToolkit::OpenBaseConverter()
{
	auto converter = new BaseConverter;
	GetCurrentTab()->AddWidget(converter, "Base converter");
}

void HxNxToolkit::OpenCalculator()
{
	auto calculator = new Calculator;
	GetCurrentTab()->AddWidget(calculator, "Calculator");
}

void HxNxToolkit::OpenChecklist()
{
	auto checklist = new Checklist;
	GetCurrentTab()->AddWidget(checklist, "Checklist");
}

void HxNxToolkit::OpenStopwatch()
{
	auto stopwatch = new Stopwatch;
	GetCurrentTab()->AddWidget(stopwatch, "Stopwatch");
}

void HxNxToolkit::OpenTimer()
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

	GetCurrentTab()->AddWidget(timer, "Timer");
}
