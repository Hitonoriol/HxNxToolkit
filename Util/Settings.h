#pragma once
#include <QSettings>

class Settings
{
public:
	static void SetString(const QString& key, const QString& value);
	static QString GetString(const QString& key);

	static const QString
		LastSaveDir;

private:
	static QSettings& Instance();
};

