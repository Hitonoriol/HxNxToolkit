#pragma once

#include <QWidget>

#include "Enums/ToolType.h"

#include <QJsonObject>
#include <QPointer>

class ComponentContainer;

class Component : public QWidget
{
	Q_OBJECT
	friend class ComponentContainer;

public:
	Component(QWidget* parent = nullptr, ToolType type = ToolType::None);
	virtual ~Component();

	ToolType GetType();

	virtual QJsonObject SaveState();
	virtual void LoadState(const QJsonObject& state);

signals:
	void Modified(Component* component);

private:
	ToolType type;
	QPointer<ComponentContainer> container;
};

