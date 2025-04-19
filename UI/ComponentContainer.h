#pragma once

#include <QWidget>
#include "ui_ComponentContainer.h"

#include "Component.h"

#include <QPointer>

class ComponentContainer : public QWidget
{
	Q_OBJECT

public:
	ComponentContainer(Component* component, QWidget *parent = nullptr);
	~ComponentContainer();

	QJsonObject SaveState();
	void LoadState(const QJsonObject& state);

	void setTitle(const QString& title);
	QString getTitle();

signals:
	void CloseClicked();
	void UpClicked();
	void DownClicked();

private slots:
	void OnTitleContextMenuRequested(const QPoint& pos);

	void OnRenameTriggered();

private:
	Ui::ComponentContainerClass ui;
	QPointer<Component> component;
};
