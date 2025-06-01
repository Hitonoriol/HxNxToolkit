#pragma once

#include <QWidget>
#include <QClipboard>
#include "ui_ClipboardHistory.h"

#include "UI/Component.h"

#include <QMetaType>
#include <filesystem>

struct ImageItem {
	QUrl url;
};

Q_DECLARE_METATYPE(ImageItem)

class ClipboardHistory : public Component
{
	Q_OBJECT

public:
	ClipboardHistory(QWidget *parent = nullptr);
	~ClipboardHistory();

	virtual QJsonObject SaveState() override;
	virtual void LoadState(const QJsonObject& state) override;

private slots:
	void OnClipboardChanged(QClipboard::Mode mode);
	void OnSelectedItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void OnItemDoubleClick(QListWidgetItem* item);
	void OnClearHistoryClicked();

private:
	Ui::ClipboardHistoryClass ui;
	bool ignoreCopyEvent = false;

	std::filesystem::path clipboardPath;
};

