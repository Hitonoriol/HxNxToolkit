#pragma once

#include <QWidget>
#include "ui_Tab.h"

#include "UI/Component.h"
#include "Enums/ToolType.h"

#include <QJsonObject>

class Tab : public QWidget
{
	Q_OBJECT

public:
	Tab(QWidget* parent = nullptr);
	~Tab();

	void SetSavePath(const QString& savePath);
	QString GetSavePath() const;

	void AddComponent(Component* component, const QString& title = "Tool");

	QJsonObject SaveState();
	void LoadState(const QJsonObject& state);

signals:
	void LoadComponent(ToolType componentType, const QJsonObject& state);

private:
	Ui::TabClass ui;

	QString savePath;
};
