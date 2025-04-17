#pragma once
#include <QWidget>
#include "ui_MarkdownEditor.h"

#include "UI/Component.h"

namespace maddy { class Parser; }

class MarkdownEditor : public Component
{
	Q_OBJECT

public:
	MarkdownEditor(QWidget *parent = nullptr);
	~MarkdownEditor();

	virtual QJsonObject SaveState() override;
	virtual void LoadState(const QJsonObject& state) override;

private:
	void onEditorTextChanged();

	Ui::MarkdownEditorClass ui;
	std::unique_ptr<maddy::Parser> markdownParser;
};
