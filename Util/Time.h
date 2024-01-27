#pragma once

#include <QString>
#include <QDateTime>

class Time
{
public:
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
	static int64_t Now();
};

