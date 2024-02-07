#pragma once
#include <QSettings>

enum class Option
{
	LastSaveDir,
	AlwaysOnTop
};

class Settings
{
public:
	static void Set(Option key, const QVariant& value);

	static QString GetString(Option key);
	static bool GetBool(Option key);

private:
	static QSettings& Instance();
};

