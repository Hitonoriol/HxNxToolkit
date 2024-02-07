#include "Settings.h"

#include <nameof/nameof.hpp>

QSettings& Settings::Instance()
{
	static QSettings instance("hxnxtk_config.ini", QSettings::IniFormat);
	return instance;
}

void Settings::Set(Option key, const QVariant& value)
{
	Instance().setValue(NAMEOF(key).c_str(), value);
}

QString Settings::GetString(Option key)
{
	return Instance().value(NAMEOF(key).c_str(), QString{}).toString();
}

bool Settings::GetBool(Option key)
{
	return Instance().value(NAMEOF(key).c_str(), false).toBool();
}
