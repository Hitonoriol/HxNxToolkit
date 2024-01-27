#include "Stopwatch.h"

#include "Util/Time.h"

#include <QDateTime>
#include <QTimeZone>

Stopwatch::Stopwatch(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(&updateTimer, &QTimer::timeout, this, &Stopwatch::UpdateTimerTick);
}

Stopwatch::~Stopwatch()
{}

void Stopwatch::OnStartPausePress()
{
	if (!updateTimer.isActive()) {
		startTime = Time::Now();
		updateTimer.start(std::chrono::milliseconds{100});
		ui.StartPauseButton->setText("Pause");
		ui.TimePointButton->setEnabled(true);
	} else {
		updateTimer.stop();
		savedTime += Elapsed();
		ui.StartPauseButton->setText("Resume");
		ui.TimePointButton->setEnabled(false);
	}
}

void Stopwatch::OnStopPress()
{
	if (updateTimer.isActive()) {
		OnStartPausePress();
	}
	ui.StartPauseButton->setText("Start");

	savedTime = 0;
	ui.TimeLabel->setText(Time::GetTimeString(0));
	ui.TimeTable->setRowCount(0);
	ui.TimePointButton->setEnabled(false);
}

void Stopwatch::OnTimePointPress()
{
	auto now = savedTime + Elapsed();
	auto diff = now - lastPoint;
	lastPoint = now;

	auto timeStr = Time::GetTimeString(now);
	auto diffStr = Time::GetTimeString(diff);

	auto newIdx = ui.TimeTable->rowCount();
	ui.TimeTable->insertRow(newIdx);
	ui.TimeTable->setItem(newIdx, 0, new QTableWidgetItem(timeStr));
	ui.TimeTable->setItem(newIdx, 1, new QTableWidgetItem("+" + diffStr));

}

void Stopwatch::UpdateTimerTick()
{
	auto timeStr = Time::GetTimeString(savedTime + Elapsed());
	ui.TimeLabel->setText(timeStr);
}

int64_t Stopwatch::Elapsed()
{
	return Time::Now() - startTime;
}
