#pragma once

#include <QWidget>
#include "ui_ComponentContainer.h"

#include "Component.h"

class ComponentContainer : public QWidget
{
	Q_OBJECT

public:
	ComponentContainer(Component* component, QWidget *parent = nullptr);
	~ComponentContainer();

	void setTitle(const QString& title);
	QString getTitle();

signals:
	void CloseClicked();

private:
	Ui::ComponentContainerClass ui;
};
