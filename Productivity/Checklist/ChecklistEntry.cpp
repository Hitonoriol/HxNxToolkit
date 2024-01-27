#include "ChecklistEntry.h"

ChecklistEntry::ChecklistEntry(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ChecklistEntry::~ChecklistEntry()
{}

void ChecklistEntry::OnDeletePressed()
{
	this->deleteLater();
}

void ChecklistEntry::OnCheckboxValueChange(bool value)
{
	ui.DescriptionField->setEnabled(!value);
	emit CheckboxValueChanged(value);
}

void ChecklistEntry::OnDescriptionChange(QString text)
{
	emit DescriptionChanged(text);
}