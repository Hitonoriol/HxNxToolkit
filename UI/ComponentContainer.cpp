#include "ComponentContainer.h"

ComponentContainer::ComponentContainer(Component* component, QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	component->setParent(this);
	ui.Content->layout()->addWidget(component);

	connect(ui.CloseBtn, &QPushButton::clicked, this, &ComponentContainer::CloseClicked);
}

ComponentContainer::~ComponentContainer()
{
}

void ComponentContainer::setTitle(const QString& title)
{
	ui.TitleLabel->setText(title);
}

QString ComponentContainer::getTitle()
{
	return ui.TitleLabel->text();
}