#include "Tab.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QSpacerItem>

Tab::Tab(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

Tab::~Tab()
{}

void Tab::AddWidget(QWidget* parent, const QString& title)
{
	auto scrollLayout = ui.Scroll->widget()->layout();
	auto group = new QGroupBox(title);
	auto layout = new QVBoxLayout;
	layout->addWidget(parent);
	group->setLayout(layout);
	scrollLayout->removeItem(ui.BottomSpacer);
	scrollLayout->addWidget(group);
	scrollLayout->addItem(ui.BottomSpacer);
}
