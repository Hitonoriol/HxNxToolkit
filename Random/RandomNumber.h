#pragma once

#include <QWidget>
#include "ui_RandomNumber.h"
#include "UI/Component.h"

#include <QRandomGenerator>
#include <random>

class RandomNumber : public Component
{
	Q_OBJECT

public:
	RandomNumber(QWidget *parent = nullptr);
	~RandomNumber();

	virtual QJsonObject SaveState() override;
	virtual void LoadState(const QJsonObject& state) override;

	enum class NumberType
	{
		Integer, Double
	};
	Q_ENUM(NumberType)

private:
	RandomNumber::NumberType GetSelectedNumberType();

private slots:
	void OnGenerateButtonPress();
	void OnNumberTypeBoxChange(int);
	void OnSpinBoxChange();

private:
	Ui::RandomNumberClass ui;
	std::mt19937 rng{std::random_device{}()};
};
