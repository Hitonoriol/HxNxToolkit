#include "MarkdownEditor.h"

#include <3rdParty/maddy/parser.h>
#include <sstream>

MarkdownEditor::MarkdownEditor(QWidget *parent)
	: Component(parent, ToolType::MarkdownEditor)
	, markdownParser(std::make_unique<maddy::Parser>())
{
	ui.setupUi(this);
	connect(ui.EditorField, &QPlainTextEdit::textChanged, this, &MarkdownEditor::onEditorTextChanged);
}

MarkdownEditor::~MarkdownEditor()
{}

void MarkdownEditor::onEditorTextChanged()
{
	std::stringstream markdown(ui.EditorField->toPlainText().toStdString());
	auto html = markdownParser->Parse(markdown);
	ui.MarkdownField->setHtml(html.data());
}