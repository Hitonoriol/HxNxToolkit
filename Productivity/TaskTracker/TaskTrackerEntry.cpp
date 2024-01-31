#include "TaskTrackerEntry.h"

#include "Util/Time.h"

TaskTrackerEntry::TaskTrackerEntry(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

TaskTrackerEntry::~TaskTrackerEntry()
{}

void TaskTrackerEntry::SetNumber(uint64_t number)
{
	ui.IndexLabel->setText(QString::number(number) + ".");
}

uint64_t TaskTrackerEntry::GetNumber()
{
	return ui.IndexLabel->text().removeLast().toULongLong();
}

void TaskTrackerEntry::SetStartTime(const QString& time)
{
	ui.StartField->setText(time);
}

QString TaskTrackerEntry::GetStartTime() const
{
	return ui.StartField->text();
}

void TaskTrackerEntry::SetEndTime(const QString& time)
{
	ui.EndField->setText(time);
}

QString TaskTrackerEntry::GetEndTime() const
{
	return ui.EndField->text();
}

QString TaskTrackerEntry::GetDescription() const
{
	return ui.DescriptionField->text();
}

void TaskTrackerEntry::SetDescription(const QString& description)
{
	ui.DescriptionField->setText(description);
}

void TaskTrackerEntry::OnEndButtonPress()
{
	auto time = QDateTime::fromString(ui.StartField->text(), "hh:mm");
	auto startTime = time.toMSecsSinceEpoch()/*Time::Parse(ui.StartField->text()).GetTimeMs()*/;
	auto endTime = Time::Now();

	auto diff = endTime - startTime;
	float hours = ((diff / 1000) % 86400) / 3600.0f;
	if (diff <= 0) {
		return;
	}

	ui.EndField->setText(QDateTime::fromMSecsSinceEpoch(endTime).toString("hh:mm"));
	ui.DurationField->setText(Time::GetTimeString(diff));
	ui.DurationHoursField->setText(QString::number(hours, 'f', 2));
	ui.EndButton->setVisible(false);

	emit EndButtonPressed();
}