#include "Checklist.h"
#include "ChecklistEntry.h"

Checklist::Checklist(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

Checklist::~Checklist()
{}

void Checklist::AddEntryTriggered()
{
	auto layout = ui.verticalLayout;
	layout->removeWidget(ui.AddEntryButton);

	auto entry = new ChecklistEntry;
	layout->addWidget(entry);
	layout->addWidget(ui.AddEntryButton);
}