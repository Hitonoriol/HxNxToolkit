#pragma once

#include <QWidget>

#include "Enums/ToolType.h"

#include <QJsonObject>

class Component : public QWidget
{
	Q_OBJECT

public:
	Component(QWidget* parent = nullptr, ToolType type = ToolType::None);
	virtual ~Component();

	ToolType GetType();

	virtual QJsonObject SaveState();
	virtual void LoadState(const QJsonObject& state);

private:
	ToolType type;
};

