#include "Timer.h"

#include "Util/Time.h"

Timer::Timer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(&updateTimer, &QTimer::timeout, this, &Timer::OnUpdateTimerTick);
}

Timer::~Timer()
{}

void Timer::OnStartButtonPress()
{
	if (!updateTimer.isActive()) {
		auto hours = ui.HourBox->value();
		auto minutes = ui.MinuteBox->value();
		auto seconds = ui.SecondBox->value();
		if (hours == 0 && minutes == 0 && seconds == 0) {
			return;
		}

		duration.SetTime(hours, minutes, seconds);
		startTime = Time::Now();
		updateTimer.start(std::chrono::milliseconds{100});
		ui.TimeEdit->setEnabled(false);
		ui.StartPauseButton->setText("Pause");
	} else {
		savedTime += qMax(0, Time::Now() - startTime);
		updateTimer.stop();
		ui.StartPauseButton->setText("Resume");
	}
}

void Timer::OnStopButtonPress()
{
	if (updateTimer.isActive()) {
		updateTimer.stop();
	}

	savedTime = 0;
	ui.TimeEdit->setEnabled(true);
	ui.StartPauseButton->setText("Start");
	ui.TimeLabel->setText(Time::GetTimeString(0));
	ui.TimeProgress->setValue(0);
}

void Timer::OnUpdateTimerTick()
{
	auto now = Time::Now();
	auto durationMs = duration.GetTimeMs();
	auto remaining = qMax(0, durationMs - ((now - startTime) + savedTime));
	auto progress = (int)(100.0f - (((float)remaining / (float)durationMs) * 100.0f));

	ui.TimeLabel->setText(Time::GetTimeString(remaining));
	ui.TimeProgress->setValue(progress);

	if (remaining == 0) {
		updateTimer.stop();
		savedTime = 0;
		ui.TimeEdit->setEnabled(true);
		ui.StartPauseButton->setText("Start");
		emit TimerCompleted(startTime, duration);
	}
}