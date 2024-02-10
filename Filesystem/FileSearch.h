#pragma once

#include <QWidget>
#include "ui_FileSearch.h"

#include "UI/Component.h"

#include <vector>
#include <filesystem>
#include <functional>
#include <thread>

class FileSearch : public Component
{
	Q_OBJECT

public:
	FileSearch(QWidget *parent = nullptr);
	~FileSearch();

	virtual QJsonObject SaveState() override;
	virtual void LoadState(const QJsonObject& state) override;

signals:
	void FileFound(std::filesystem::path foundPath);
	void ContentsFound(std::filesystem::path foundPath, size_t line, QString context);
	void SearchFinished();
	void FilesWalked(size_t count);

private slots:
	void OnBrowsePress();
	void OnSearchPress();
	void OnStopPress();

	void OnFileFound(std::filesystem::path foundPath);
	void OnContentsFound(std::filesystem::path foundPath, size_t line, QString context);
	void OnSearchFinished();
	void OnFilesWalked(size_t count);

	void OnNameSearchSelected();
	void OnContentsSearchSelected();

	void OnSearchResultDoubleClicked(QListWidgetItem* item);
	void OnSearchResultDoubleClicked(QTableWidgetItem* item);

private:
	void FindFiles(
		const QString& baseDirPath,
		const QString& searchPattern);

	void FindContents(
		const QString& baseDirPath,
		const QString& searchPattern);

	void PrepareForSearch();

	void OpenInExplorer(const QString& pathStr);

	Ui::FileSearchClass ui;
	std::thread searchThread;
	std::atomic<bool> stop{};
};
