#pragma once

#include <QWidget>
#include "ui_ChecklistEntry.h"

class ChecklistEntry : public QWidget
{
	Q_OBJECT

public:
	ChecklistEntry(QWidget *parent = nullptr);
	~ChecklistEntry();

signals:
	void CheckboxValueChanged(bool value);
	void DescriptionChanged(QString text);

private slots:
	void OnCheckboxValueChange(bool value);
	void OnDescriptionChange(QString text);

	void OnDeletePressed();

private:
	Ui::ChecklistEntryClass ui;
};
