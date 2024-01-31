#include "Component.h"

Component::Component(QWidget* parent, ToolType type)
	: QWidget(parent), type(type)
{
}

Component::~Component()
{
}

ToolType Component::GetType()
{
	return type;
}

QJsonObject Component::SaveState()
{
	QJsonObject state;
	state["Type"] = static_cast<int>(type);
	return state;
}

void Component::LoadState(const QJsonObject& state)
{
}
