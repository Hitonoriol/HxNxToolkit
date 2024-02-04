#include "RandomString.h"

RandomString::RandomString(QWidget *parent)
	: Component(parent, ToolType::RandomString)
{
	ui.setupUi(this);
}

RandomString::~RandomString()
{}

QJsonObject RandomString::SaveState()
{
	auto state = Component::SaveState();
	state["CharacterSet"] = ui.CharacterSetField->text();
	state["Length"] = ui.LengthBox->value();
	state["StringNumber"] = ui.StringNumberBox->value();
	state["Output"] = ui.OutputField->toPlainText();
	return state;
}

void RandomString::LoadState(const QJsonObject& state)
{
	ui.CharacterSetField->setText(state["CharacterSet"].toString());
	ui.LengthBox->setValue(state["Length"].toInt());
	ui.StringNumberBox->setValue(state["StringNumber"].toInt());
	ui.OutputField->setPlainText(state["Output"].toString());
}

void RandomString::OnGenerateButtonPress()
{
	ui.OutputField->clear();
	auto strCount = ui.StringNumberBox->value();
	for (size_t i = 0; i < strCount; ++i) {
		ui.OutputField->appendPlainText(GenerateString());
	}

	emit Modified(this);
}

void RandomString::OnCharacterSetChange()
{
	emit Modified(this);
}

void RandomString::OnStringLengthChange()
{
	emit Modified(this);
}

QString RandomString::GenerateString()
{
	QString result;
	auto charSet = ui.CharacterSetField->text();
	if (charSet.isEmpty()) {
		return {};
	}

	auto length = ui.LengthBox->value();
	std::uniform_int_distribution distr(0, static_cast<int>(charSet.size() - 1));
	for (size_t i = 0; i < length; ++i) {
		result += charSet[distr(rng)];
	}

	return result;
}
