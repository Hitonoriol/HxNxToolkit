#pragma once

#include <QWidget>
#include "ui_Calculator.h"

class Calculator : public QWidget
{
	Q_OBJECT

public:
	Calculator(QWidget *parent = nullptr);
	~Calculator();

public slots:
	void InputTextChanged(QString text);

private:
	Ui::CalculatorClass ui;
};
