#pragma once

#include <QString>
#include <QDateTime>
#include <QTime>

class Time
{
public:
	Time(int64_t time = 0);

	void SetTime(int hours, int minutes, int seconds, int millis = 0);

	inline int64_t GetTimeMs()
	{
		return time;
	}

	inline int64_t GetMillis()
	{
		return GetMillis(time);
	}

	inline int64_t GetSeconds()
	{
		return GetSeconds(time);
	}

	inline int64_t GetMinutes()
	{
		return GetMinutes(time);
	}

	inline int64_t GetHours()
	{
		return GetHours(time);
	}

	static inline int64_t GetMillis(int64_t timeMs)
	{
		return timeMs % 1000;
	}

	static inline int64_t GetSeconds(int64_t timeMs)
	{
		return (timeMs / 1000) % 60;
	}

	static inline int64_t GetMinutes(int64_t timeMs)
	{
		return ((timeMs / 1000) % 3600) / 60;
	}

	static inline int64_t GetHours(int64_t timeMs)
	{
		return ((timeMs / 1000) % 86400) / 3600;
	}

	static QString GetTimeString(int64_t timeMs, bool withMillis = true);
	static QString GetTimeString(const QDateTime& dateTime);

	static int64_t Now();

private:
	int64_t time{};
};

