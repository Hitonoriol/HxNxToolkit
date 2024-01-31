#include "TaskTracker.h"

#include "TaskTrackerEntry.h"

#include <QJsonArray>

TaskTracker::TaskTracker(QWidget *parent)
	: Component(parent, ToolType::TaskTracker)
{
	ui.setupUi(this);
}

TaskTracker::~TaskTracker()
{}

QJsonObject TaskTracker::SaveState()
{
	auto state = Component::SaveState();
	auto entries = findChildren<TaskTrackerEntry*>();
	QJsonArray array;
	for (auto entry : entries) {
		QJsonObject entryObj;
		entryObj["StartTime"] = entry->GetStartTime();
		entryObj["EndTime"] = entry->GetEndTime();
		entryObj["Description"] = entry->GetDescription();
		array.append(entryObj);
	}
	state["Tasks"] = array;
	return state;
}

void TaskTracker::LoadState(const QJsonObject& state)
{
	auto entries = state["Tasks"].toArray();
	for (auto entry : entries) {
		auto task = AddTaskEntry();
		auto taskObj = entry.toObject();
		task->SetStartTime(taskObj["StartTime"].toString());
		task->SetEndTime(taskObj["EndTime"].toString());
		task->SetDescription(taskObj["Description"].toString());
	}
}

TaskTrackerEntry* TaskTracker::AddTaskEntry()
{
	auto nextEntry = new TaskTrackerEntry;
	connect(nextEntry, &TaskTrackerEntry::EndButtonPressed, this, [this] {
		AddTaskEntry();
	});
	ui.TaskContainer->addWidget(nextEntry);

	if (lastEntry) {
		nextEntry->SetStartTime(lastEntry->GetEndTime());
		nextEntry->SetNumber(lastEntry->GetNumber() + 1);
	}

	lastEntry = nextEntry;

	return nextEntry;
}

void TaskTracker::OnStartSessionPress()
{
	AddTaskEntry();
	ui.StartButton->setVisible(false);
	ui.DurationWidget->setEnabled(false);
}