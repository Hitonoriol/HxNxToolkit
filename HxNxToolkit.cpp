#include "HxNxToolkit.h"

#include "General/BaseConverter.h"

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
	auto title = QDateTime::currentDateTime().date().toString();
	curTabIdx = ui.Tabs->addTab(tab, title);
	return tab;
}

Tab* HxNxToolkit::GetCurrentTab()
{
	return dynamic_cast<Tab*>(ui.Tabs->currentWidget());
}

void HxNxToolkit::OpenBaseConverter()
{
	auto converter = new BaseConverter;
	GetCurrentTab()->AddWidget(converter, "Base converter");
}