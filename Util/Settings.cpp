#include "Settings.h"

const QString
Settings::LastSaveDir = "LastSaveDir";

QSettings& Settings::Instance()
{
	static QSettings instance("hxnxtk_config.ini", QSettings::IniFormat);
	return instance;
}

void Settings::SetString(const QString& key, const QString& value)
{
	Instance().setValue(key, value);
}

QString Settings::GetString(const QString& key)
{
	return Instance().value(key, QString{}).toString();
}