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

QLineEdit* TaskTrackerEntry::GetEndField()
{
	return ui.EndField;
}

QPushButton* TaskTrackerEntry::GetEndButton()
{
	return ui.EndButton;
}

void TaskTrackerEntry::OnEndFieldModified(QString newTime)
{
	if (ui.EndButton->isVisible()) {
		return;
	}

	UpdateTime();

	emit EndFieldModified(newTime);
}

void TaskTrackerEntry::OnStartFieldModified(QString newTime)
{
	if (ui.EndButton->isVisible()) {
		return;
	}

	UpdateTime();

	emit StartFieldModified(newTime);
}

void TaskTrackerEntry::UpdateTime(int64_t begin, int64_t end, bool updateEndField)
{
	auto diff = end - begin;
	float hours = ((diff / 1000) % 86400) / 3600.0f;
	if (diff <= 0) {
		return;
	}

	if (updateEndField) {
		ui.EndField->setText(QDateTime::fromMSecsSinceEpoch(end).toString("hh:mm"));
	}

	ui.DurationField->setText(Time::GetTimeString(diff));
	ui.DurationHoursField->setText(QString::number(hours, 'f', 2));
}

void TaskTrackerEntry::UpdateTime()
{
	try {
		auto start = QDateTime::fromString(ui.StartField->text(), "hh:mm").toMSecsSinceEpoch();
		auto end = QDateTime::fromString(ui.EndField->text(), "hh:mm").toMSecsSinceEpoch();
		UpdateTime(start, end, false);
	}
	catch (...) {
		ui.DurationField->setText("");
		ui.DurationHoursField->setText("");
	}
}

void TaskTrackerEntry::OnEndButtonPress()
{
	auto time = QDateTime::fromString(ui.StartField->text(), "hh:mm");
	auto startTime = time.toMSecsSinceEpoch();
	auto endTime = Time::Now();

	UpdateTime(startTime, endTime);
	ui.EndButton->setVisible(false);
	ui.EndField->setReadOnly(false);
	emit EndButtonPressed();
}