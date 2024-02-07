#include "Settings.h"

#include <nameof/nameof.hpp>

QSettings& Settings::Instance()
{
	static QSettings instance("hxnxtk_config.ini", QSettings::IniFormat);
	return instance;
}

void Settings::Set(Option key, const QVariant& value)
{
	Instance().setValue(nameof::nameof_enum(key), value);
	Instance().sync();
}

void Settings::SetDefault(Option key, const QVariant& value)
{
	auto& instance = Instance();
	auto name = nameof::nameof_enum(key);
	if (!instance.contains(name)) {
		instance.setValue(name, value);
	}
}

QString Settings::GetString(Option key)
{
	return Instance().value(nameof::nameof_enum(key), QString{}).toString();
}

int Settings::GetInt(Option key)
{
	return Instance().value(nameof::nameof_enum(key), 0).toInt();
}

bool Settings::GetBool(Option key)
{
	return Instance().value(nameof::nameof_enum(key), false).toBool();
}
