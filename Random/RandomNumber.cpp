#include "RandomNumber.h"

#include <QMetaEnum>

RandomNumber::RandomNumber(QWidget *parent)
	: Component(parent, ToolType::RandomNumber)
{
	ui.setupUi(this);
	ui.MinIntBox->setRange(INT_MIN, INT_MAX);
	ui.MaxIntBox->setRange(INT_MIN, INT_MAX);
	ui.MinDoubleBox->setRange(DBL_MIN, DBL_MAX);
	ui.MaxDoubleBox->setRange(DBL_MIN, DBL_MAX);

	auto numberEnum = QMetaEnum::fromType<RandomNumber::NumberType>();
	for (size_t i = 0; i < numberEnum.keyCount(); ++i) {
		ui.NumberTypeBox->addItem(numberEnum.key(i));
	}

	OnNumberTypeBoxChange(0);
}

RandomNumber::~RandomNumber()
{}

QJsonObject RandomNumber::SaveState()
{
	auto state = Component::SaveState();
	state["MinInt"] = ui.MinIntBox->value();
	state["MaxInt"] = ui.MaxIntBox->value();
	state["MinDouble"] = ui.MinDoubleBox->value();
	state["MaxDouble"] = ui.MaxDoubleBox->value();
	state["NumberCount"] = ui.NumberCountBox->value();
	state["NumberType"] = ui.NumberTypeBox->currentIndex();
	state["Output"] = ui.OutputField->toPlainText();
	return state;
}

void RandomNumber::LoadState(const QJsonObject& state)
{
	ui.MinIntBox->setValue(state["MinInt"].toInt());
	ui.MaxIntBox->setValue(state["MaxInt"].toInt());
	ui.MinDoubleBox->setValue(state["MinDouble"].toDouble());
	ui.MaxDoubleBox->setValue(state["MaxDouble"].toDouble());
	ui.NumberCountBox->setValue(state["NumberCount"].toInt());
	ui.NumberTypeBox->setCurrentIndex(state["NumberType"].toInt());
	ui.OutputField->setPlainText(state["Output"].toString());
}

RandomNumber::NumberType RandomNumber::GetSelectedNumberType()
{
	return static_cast<RandomNumber::NumberType>(ui.NumberTypeBox->currentIndex());
}

void RandomNumber::OnNumberTypeBoxChange(int newIndex)
{
	auto type = static_cast<RandomNumber::NumberType>(newIndex);
	switch (type) {
	case RandomNumber::NumberType::Integer:
		ui.DoubleWidget->setVisible(false);
		ui.IntegerWidget->setVisible(true);
		break;

	case RandomNumber::NumberType::Double:
		ui.DoubleWidget->setVisible(true);
		ui.IntegerWidget->setVisible(false);
		break;

	default:
		break;
	}

	emit Modified(this);
}

void RandomNumber::OnSpinBoxChange()
{
	emit Modified(this);
}

void RandomNumber::OnGenerateButtonPress()
{
	ui.OutputField->clear();

	auto nums = ui.NumberCountBox->value();
	switch (GetSelectedNumberType()) {
	case RandomNumber::NumberType::Integer:
	{
		auto min = ui.MinIntBox->value();
		auto max = ui.MaxIntBox->value();
		std::uniform_int_distribution distr(min, max);
		for (size_t i = 0; i < nums; ++i) {
			auto num = distr(rng);
			ui.OutputField->appendPlainText(QString::number(num));
		}
		break;
	}

	case RandomNumber::NumberType::Double:
	{
		auto min = ui.MinDoubleBox->value();
		auto max = ui.MaxDoubleBox->value();
		std::uniform_real_distribution distr(min, max);
		for (size_t i = 0; i < nums; ++i) {
			auto num = distr(rng);
			ui.OutputField->appendPlainText(QString::number(num));
		}
		break;
	}

	default:
		break;
	}

	emit Modified(this);
}