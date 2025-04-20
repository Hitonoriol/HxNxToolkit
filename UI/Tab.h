#pragma once

#include <QWidget>
#include "ui_Tab.h"

#include "UI/Component.h"
#include "Enums/ToolType.h"

#include <QJsonObject>

class Tab : public QWidget
{
	Q_OBJECT

public:
	enum class ExpandMode {
		MinSize,
		Fill
	};

	Tab(QWidget* parent = nullptr);
	~Tab();

	void SetSavePath(const QString& savePath);
	QString GetSavePath() const;

	void AddComponent(Component* component, const QString& title = "Tool");

	QJsonObject SaveState();
	void LoadState(const QJsonObject& state);

	bool IsModified();

	// For external modifications only
	void Modify();

	ExpandMode GetExpandMode();
	void SetExpandMode(ExpandMode mode);

signals:
	void LoadComponent(ToolType componentType, const QJsonObject& state);

	void TabModified(Tab* tab);
	void TabSaved(Tab* tab);

private slots:
	void ComponentModified(Component* component);
	void OnComponentClosed(ComponentContainer* component);
	void OnComponentMoved(ComponentContainer* component, int direction);

private:
	void UpdateBottomSpacer();

	Ui::TabClass ui;

	ExpandMode expandMode = ExpandMode::MinSize;
	QString savePath;
	bool modified{};
};
