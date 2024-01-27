#include "HxNxToolkit.h"

#include "General/BaseConverter.h"
#include "General/Calculator.h"

#include "Productivity/Checklist/Checklist.h"

#include <QDateTime>

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
	auto title = QDateTime::currentDateTime().toString();
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