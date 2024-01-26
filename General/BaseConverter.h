#pragma once

#include <QWidget>
#include "ui_BaseConverter.h"

class BaseConverter : public QWidget
{
	Q_OBJECT

public:
	BaseConverter(QWidget *parent = nullptr);
	~BaseConverter();

	enum class Base
	{
		Dec = 10,
		Hex = 16,
		Bin = 2
	};
	Q_ENUM(Base)

public slots:
	void InputBaseChanged(int idx);
	void OutputBaseChanged(int idx);
	void InputTextChanged(QString text);

private:
	QString Convert(const QString& value, Base src, Base dst);

	Ui::BaseConverterClass ui;
};
