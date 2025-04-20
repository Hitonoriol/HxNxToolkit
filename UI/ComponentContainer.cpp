#include "ComponentContainer.h"

#include <QMenu>
#include <QInputDialog>

ComponentContainer::ComponentContainer(Component* component, QWidget* parent)
	: QWidget(parent)
	, component(component)
{
	ui.setupUi(this);
	component->container = this;
	ui.Content->layout()->addWidget(component);
	ui.Title->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

	connect(ui.CloseBtn, &QPushButton::clicked, this, &ComponentContainer::CloseClicked);
	connect(ui.UpBtn, &QPushButton::clicked, this, &ComponentContainer::UpClicked);
	connect(ui.DownBtn, &QPushButton::clicked, this, &ComponentContainer::DownClicked);
	connect(ui.CollapseBtn, &QPushButton::clicked, this, &ComponentContainer::OnCollapseClicked);
	connect(ui.Title, &QWidget::customContextMenuRequested, this, &ComponentContainer::OnTitleContextMenuRequested);
}

ComponentContainer::~ComponentContainer()
{
}

QJsonObject ComponentContainer::SaveState()
{
	QJsonObject state;

	state["Title"] = getTitle();

	return state;
}

void ComponentContainer::LoadState(const QJsonObject& state)
{
	setTitle(state["Title"].toString());
}

void ComponentContainer::setTitle(const QString& title)
{
	ui.TitleLabel->setText(title);
}

QString ComponentContainer::getTitle()
{
	return ui.TitleLabel->text();
}

void ComponentContainer::OnCollapseClicked()
{
	if (ui.Content->isVisible()) {
		ui.Content->hide();
		ui.CollapseBtn->setText("\uE740");
		emit CollapseClicked(true);
	}
	else {
		ui.Content->show();
		ui.CollapseBtn->setText("\uE73F");
		emit CollapseClicked(false);
	}
}

void ComponentContainer::OnTitleContextMenuRequested(const QPoint& pos)
{
	QMenu menu(this);

	connect(menu.addAction("Rename"), &QAction::triggered, this, &ComponentContainer::OnRenameTriggered);

	menu.exec(ui.Title->mapToGlobal(pos));
}

void ComponentContainer::OnRenameTriggered()
{
	bool entered = false;
	auto newTitle = QInputDialog::getText(this, "Rename component", "Component title:", QLineEdit::Normal, getTitle(), &entered);

	if (entered && !newTitle.isEmpty()) {
		setTitle(newTitle);
		emit component->Modified(component);
	}
}