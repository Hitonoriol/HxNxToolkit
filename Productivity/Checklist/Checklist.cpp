#include "Checklist.h"
#include "ChecklistEntry.h"

#include <QJsonArray>

Checklist::Checklist(QWidget *parent)
	: Component(parent, ToolType::Checklist)
{
	ui.setupUi(this);
}

Checklist::~Checklist()
{}

QJsonObject Checklist::SaveState()
{
	auto state = Component::SaveState();
	auto entries = findChildren<ChecklistEntry*>();

	QJsonArray array;
	for (auto entry : entries) {
		QJsonObject entryObj;
		entryObj["Checked"] = entry->IsChecked();
		entryObj["Description"] = entry->GetDescription();
		array.append(entryObj);
	}

	state["Entries"] = array;
	return state;
}

void Checklist::LoadState(const QJsonObject& state)
{
	auto entries = state["Entries"].toArray();
	for (auto entry : entries) {
		auto entryObj = entry.toObject();
		auto newEntry = AddEntry();
		newEntry->SetChecked(entryObj["Checked"].toBool());
		newEntry->SetDescription(entryObj["Description"].toString());
	}
}

ChecklistEntry* Checklist::AddEntry()
{
	auto layout = ui.verticalLayout;
	layout->removeWidget(ui.AddEntryButton);

	auto entry = new ChecklistEntry;
	layout->addWidget(entry);
	layout->addWidget(ui.AddEntryButton);

	return entry;
}

void Checklist::AddEntryTriggered()
{
	AddEntry();
}