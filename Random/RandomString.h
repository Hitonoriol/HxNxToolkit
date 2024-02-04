#pragma once

#include <QWidget>
#include "ui_RandomString.h"

#include "UI/Component.h"
#include <random>

class RandomString : public Component
{
	Q_OBJECT

public:
	RandomString(QWidget *parent = nullptr);
	~RandomString();

	virtual QJsonObject SaveState() override;
	virtual void LoadState(const QJsonObject& state) override;

private slots:
	void OnGenerateButtonPress();
	void OnCharacterSetChange();
	void OnStringLengthChange();

private:
	QString GenerateString();

	Ui::RandomStringClass ui;
	std::mt19937 rng{std::random_device{}()};
};
