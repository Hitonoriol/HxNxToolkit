#include "RandomString.h"
#include "JSON/Serialization.h"

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
	HX_JSON_SCOPE(state,
		HX_SERIALIZE(ui.CharacterSetField, text);
	    HX_SERIALIZE(ui.LengthBox, value);
		HX_SERIALIZE(ui.StringNumberBox, value);
		HX_SERIALIZE(ui.OutputField, toPlainText);
	)
	return state;
}

void RandomString::LoadState(const QJsonObject& state)
{
	Component::LoadState(state);

	HX_JSON_SCOPE(state,
		HX_DESERIALIZE(ui.CharacterSetField, setText, toString);
	    HX_DESERIALIZE(ui.LengthBox, setValue, toInt);
	    HX_DESERIALIZE(ui.StringNumberBox, setValue, toInt);
	    HX_DESERIALIZE(ui.OutputField, setPlainText, toString);
	)
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
