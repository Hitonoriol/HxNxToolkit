#pragma once

#include <QWidget>
#include "ui_BaseConverter.h"

#include "UI/Component.h"

class BaseConverter : public Component
{
	Q_OBJECT

public:
	BaseConverter(QWidget *parent = nullptr);
	~BaseConverter();

	virtual QJsonObject SaveState() override;
	virtual void LoadState(const QJsonObject& state) override;

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
