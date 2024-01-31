#include "Tab.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QJsonArray>

Tab::Tab(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

Tab::~Tab()
{}

void Tab::ComponentModified(Component* component)
{
	modified = true;
}

void Tab::SetSavePath(const QString& savePath)
{
	this->savePath = savePath;
}

QString Tab::GetSavePath() const
{
	return savePath;
}

void Tab::AddComponent(Component* component, const QString& title)
{
	auto scrollLayout = ui.Scroll->widget()->layout();

	// TODO: Move this to a separate widget
	auto group = new QGroupBox(title);
	auto layout = new QVBoxLayout;
	layout->addWidget(component);
	group->setLayout(layout);

	scrollLayout->removeItem(ui.BottomSpacer);
	scrollLayout->addWidget(group);
	scrollLayout->addItem(ui.BottomSpacer);

	connect(component, &Component::Modified, this, &Tab::ComponentModified);
}

QJsonObject Tab::SaveState()
{
	QJsonObject state;
	QJsonArray arr;
	auto scrollLayout = ui.Scroll->widget();
	auto components = scrollLayout->findChildren<Component*>();
	for (auto& component : components) {
		arr.append(component->SaveState());
	}
	state["Components"] = arr;
	modified = false;
	return state;
}

void Tab::LoadState(const QJsonObject& state)
{
	auto componentArr = state["Components"].toArray();
	for (auto componentRef : componentArr) {
		auto componentObj = componentRef.toObject();
		auto componentType = static_cast<ToolType>(componentObj["Type"].toInt());
		emit LoadComponent(componentType, componentObj);
	}
}

bool Tab::IsModified()
{
	return modified;
}
