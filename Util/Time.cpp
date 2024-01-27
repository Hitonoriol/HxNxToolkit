#include "Time.h"

QString Time::GetTimeString(int64_t timeMs, bool withMillis)
{
	timeMs = qMax(0, timeMs);
	auto s = GetSeconds(timeMs);
	auto m = GetMinutes(timeMs);
	auto h = GetHours(timeMs);

	auto pad = [](const QString& t) { return t.size() < 2 ? ("0" + t) : t; };

	QString timeStr =
		pad(QString::number(h)) + ":"
		+ pad(QString::number(m)) + ":"
		+ pad(QString::number(s));

	if (withMillis) {
		auto ms = GetMillis(timeMs);
		auto padMs = [](const QString& t) { return t.size() < 3 ? (t + QString("0").repeated(3 - t.size())) : t; };
		timeStr += "." + padMs(QString::number(ms));
	}

	return timeStr;
}

int64_t Time::Now()
{
	return QDateTime::currentMSecsSinceEpoch();
}
