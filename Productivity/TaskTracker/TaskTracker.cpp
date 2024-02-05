#include "TaskTracker.h"

#include "TaskTrackerEntry.h"
#include "Util/Time.h"

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
		entryObj["Finished"] = entry->IsFinished();
		array.append(entryObj);
	}
	state["Tasks"] = array;
	return state;
}

void TaskTracker::LoadState(const QJsonObject& state)
{
	auto entries = state["Tasks"].toArray();
	if (!entries.isEmpty()) {
		ui.StartButton->setVisible(false);
		ui.DurationWidget->setEnabled(false);
	}

	for (auto entry : entries) {
		auto task = AddTaskEntry();
		auto taskObj = entry.toObject();
		task->SetStartTime(taskObj["StartTime"].toString());
		task->SetEndTime(taskObj["EndTime"].toString());
		task->SetDescription(taskObj["Description"].toString());

		bool finished = taskObj["Finished"].toBool();
		task->GetEndButton()->setVisible(finished);
		task->GetEndField()->setReadOnly(!finished);
	}
}

TaskTrackerEntry* TaskTracker::AddTaskEntry()
{
	auto nextEntry = new TaskTrackerEntry;
	connect(nextEntry, &TaskTrackerEntry::EndButtonPressed, this, [this, nextEntry] {
		UpdateTotalTime();
		AddTaskEntry();
	});

	auto modifiedHandler = [=](auto) {
		UpdateTotalTime();
		emit Modified(this);
	};
	connect(nextEntry, &TaskTrackerEntry::EndFieldModified, this, modifiedHandler);
	connect(nextEntry, &TaskTrackerEntry::StartFieldModified, this, modifiedHandler);
	connect(nextEntry, &TaskTrackerEntry::DescriptionFieldModified, this, modifiedHandler);

	ui.TaskContainer->addWidget(nextEntry);

	if (lastEntry) {
		nextEntry->SetStartTime(lastEntry->GetEndTime());
		nextEntry->SetNumber(lastEntry->GetNumber() + 1);
		connect(lastEntry, &TaskTrackerEntry::EndFieldModified, this, [=](QString newTime) {
			nextEntry->SetStartTime(newTime);
		});
	}

	lastEntry = nextEntry;

	UpdateTotalTime();
	emit Modified(this);
	return nextEntry;
}

void TaskTracker::UpdateTotalTime()
{
	auto taskEntries = findChildren<TaskTrackerEntry*>();
	if (taskEntries.isEmpty()) {
		return;
	}

	int64_t totalDuration{};
	for (auto entry : taskEntries) {
		if (!entry->GetEndButton()->isVisible()) {
			totalDuration += entry->GetDuration();
		}
	}

	ui.TimeLabel->setText("Time spent: " + Time::GetTimeString(totalDuration));

	auto lastEntry = taskEntries.last();
	if (lastEntry && !lastEntry->IsFinished()) {
		lastEntry->GetEndButton()->setVisible(Time::GetHours(totalDuration) < ui.SessionDurationBox->value());
	}
}

void TaskTracker::OnStartSessionPress()
{
	AddTaskEntry();
	ui.StartButton->setVisible(false);
	ui.DurationWidget->setEnabled(false);
	emit Modified(this);
}