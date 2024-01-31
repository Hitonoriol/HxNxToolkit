#include "BaseConverter.h"

#include <QMetaEnum>
#include <string>
#include <sstream>
#include <comdef.h>

BaseConverter::BaseConverter(QWidget *parent)
	: Component(parent, ToolType::BaseConverter)
{
	ui.setupUi(this);

	auto baseEnum = QMetaEnum::fromType<BaseConverter::Base>();
	for (size_t i = 0; i < baseEnum.keyCount(); ++i) {
		ui.InputBaseSelector->addItem(baseEnum.key(i));
		ui.OutputBaseSelector->addItem(baseEnum.key(i));
	}
}

BaseConverter::~BaseConverter()
{}

QJsonObject BaseConverter::SaveState()
{
	auto state = Component::SaveState();
	state["InputBase"] = ui.InputBaseSelector->currentIndex();
	state["OutputBase"] = ui.OutputBaseSelector->currentIndex();
	state["InputValue"] = ui.InputField->text();
	state["OutputValue"] = ui.OutputField->text();
	return state;
}

void BaseConverter::LoadState(const QJsonObject& state)
{
	ui.InputBaseSelector->setCurrentIndex(state["InputBase"].toInt());
	ui.OutputBaseSelector->setCurrentIndex(state["OutputBase"].toInt());
	ui.InputField->setText(state["InputValue"].toString());
	ui.OutputField->setText(state["OutputValue"].toString());
}

void BaseConverter::InputTextChanged(QString text)
{
	auto baseEnum = QMetaEnum::fromType<BaseConverter::Base>();
	auto srcBase = static_cast<Base>(baseEnum.value(ui.InputBaseSelector->currentIndex()));
	auto dstBase = static_cast<Base>(baseEnum.value(ui.OutputBaseSelector->currentIndex()));

	auto result = Convert(ui.InputField->text(), srcBase, dstBase);
	ui.OutputField->setText(result);

	bool ok{};
	HRESULT hResult = result.toInt(&ok, static_cast<int>(dstBase));
	if (!ok) {
		ui.HROutputField->setText("");
		return;
	}

	_com_error comError(hResult);
	ui.HROutputField->setText(QString::fromStdWString(comError.ErrorMessage()));
}

void BaseConverter::InputBaseChanged(int)
{
	InputTextChanged(ui.InputField->text());
}

void BaseConverter::OutputBaseChanged(int)
{
	InputTextChanged(ui.InputField->text());
}

QString BaseConverter::Convert(const QString& value, Base src, Base dst)
{
	uint64_t in{};
	try {
		in = std::stoull(value.toStdString(), nullptr, static_cast<int>(src));
	}
	catch (...) {
		return {};
	}

	auto result = QString::number(in, static_cast<int>(dst));
	switch (dst) {
	case Base::Hex:
		result = "0x" + result;
		break;

	case Base::Bin:
		result = "0b" + result;
		break;
	}

	return result;
}
