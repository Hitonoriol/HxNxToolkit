#include "Tab.h"
#include "ComponentContainer.h"

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
	emit TabModified(this);
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
	auto container = new ComponentContainer(component, this);
	container->setTitle(title);

	scrollLayout->removeItem(ui.BottomSpacer);
	scrollLayout->addWidget(container);
	scrollLayout->addItem(ui.BottomSpacer);

	connect(component, &Component::Modified, this, &Tab::ComponentModified);
	connect(container, &ComponentContainer::CloseClicked, this, [container]() { container->deleteLater(); });

	emit ComponentModified(nullptr);
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

	emit TabSaved(this);
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

	emit TabSaved(this);
	modified = false;
}

bool Tab::IsModified()
{
	return modified;
}

void Tab::Modify()
{
	emit TabModified(this);
	modified = true;
}

Tab::ExpandMode Tab::GetExpandMode()
{
	return expandMode;
}

void Tab::SetExpandMode(ExpandMode mode)
{
	if (expandMode == mode) {
		return;
	}

	auto scrollLayout = ui.Scroll->widget()->layout();

	expandMode = mode;

	switch (mode) {
	case ExpandMode::MinSize:
		scrollLayout->addItem(ui.BottomSpacer);
		break;

	case ExpandMode::Fill:
		scrollLayout->removeItem(ui.BottomSpacer);
		break;

	default:
		throw std::runtime_error("Unknown ExpandMode");
		break;
	}
}
