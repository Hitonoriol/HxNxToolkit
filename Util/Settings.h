#pragma once
#include <QSettings>

enum class Option
{
	LastSaveDir,
	LastSavedTabPath,
	AlwaysOnTop,
	AutosaveInterval,
	HideWhenMinimized,
	HideWhenClosed,
	RestorePreviousSession
};

class Settings
{
public:
	static void Set(Option key, const QVariant& value);
	static void SetDefault(Option key, const QVariant& value);

	static QString GetString(Option key);
	static int GetInt(Option key);
	static bool GetBool(Option key);

private:
	static QSettings& Instance();
};

