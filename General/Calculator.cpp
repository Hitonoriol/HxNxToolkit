#include "Calculator.h"

#include <tinyexpr/tinyexpr.h>

Calculator::Calculator(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

Calculator::~Calculator()
{}

void Calculator::InputTextChanged(QString text)
{
	int error{};
	auto result = te_interp(text.toStdString().c_str(), &error);
	auto resultStr = !error ? QString::number(result, 'f', 12) : QString("");
	for (int64_t i = resultStr.size() - 1; i >= 0; --i) {
		if (resultStr[i] != '0') {
			break;
		}
		resultStr.removeAt(i);
	}

	if (!resultStr.isEmpty()) {
		auto lastIdx = resultStr.size() - 1;
		if (resultStr[lastIdx] == '.') {
			resultStr.removeAt(lastIdx);
		}
	}

	ui.OutputField->setText(resultStr);
}
