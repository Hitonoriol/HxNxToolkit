#include "Stopwatch.h"

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
		timer.restart();
		updateTimer.start(std::chrono::milliseconds{100});
		ui.StartPauseButton->setText("Pause");
		ui.TimePointButton->setEnabled(true);
	} else {
		updateTimer.stop();
		elapsed += timer.elapsed();
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

	elapsed = 0;
	ui.TimeLabel->setText(GetTimeString(0));
	ui.TimeTable->setRowCount(0);
	ui.TimePointButton->setEnabled(false);
}

void Stopwatch::OnTimePointPress()
{
	auto now = elapsed + timer.elapsed();
	auto diff = now - lastPoint;
	lastPoint = now;

	auto timeStr = GetTimeString(now);
	auto diffStr = GetTimeString(diff);

	auto newIdx = ui.TimeTable->rowCount();
	ui.TimeTable->insertRow(newIdx);
	ui.TimeTable->setItem(newIdx, 0, new QTableWidgetItem(timeStr));
	ui.TimeTable->setItem(newIdx, 1, new QTableWidgetItem("+" + diffStr));

}

void Stopwatch::UpdateTimerTick()
{
	auto timeStr = GetTimeString(elapsed + timer.elapsed());
	ui.TimeLabel->setText(timeStr);
}

QString Stopwatch::GetTimeString(int64_t timeMs)
{
	timeMs = qMax(0, timeMs);
	auto ms = timeMs % 1000;
	timeMs /= 1000;
	auto s = timeMs % 60;
	auto m = (timeMs % 3600) / 60;
	auto h = (timeMs % 86400) / 3600;

	auto pad = [](const QString& t) { return t.size() < 2 ? ("0" + t) : t; };
	auto padMs = [](const QString& t) { return t.size() < 3 ? (t + QString("0").repeated(3 - t.size())) : t; };

	QString timeStr =
		pad(QString::number(h)) + ":"
		+ pad(QString::number(m)) + ":"
		+ pad(QString::number(s)) + "."
		+ padMs(QString::number(ms));

	return timeStr;
}