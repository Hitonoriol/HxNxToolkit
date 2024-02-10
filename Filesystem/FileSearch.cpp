#include "FileSearch.h"

#include "JSON/Serialization.h"

#include <QRegularExpression>
#include <QFileDialog>

// TODO: maybe hide this behind macros for portability? if anybody even cares...
#include <windows.h>
#include <shlobj.h>

FileSearch::FileSearch(QWidget *parent)
	: Component(parent, ToolType::FileSearch)
{
	ui.setupUi(this);
	ui.WalkedFileslabel->setVisible(false);
	ui.StopButton->setVisible(false);

	connect(this, &FileSearch::FileFound, this, &FileSearch::OnFileFound);
	connect(this, &FileSearch::SearchFinished, this, &FileSearch::OnSearchFinished);
	connect(this, &FileSearch::FilesWalked, this, &FileSearch::OnFilesWalked);
}

FileSearch::~FileSearch()
{}

QJsonObject FileSearch::SaveState()
{
	auto state = Component::SaveState();
	HX_JSON_SCOPE(state,
		HX_SERIALIZE(ui.PathField, text);
	    HX_SERIALIZE(ui.SearchPatternField, text);
	);

	QJsonArray searchResults;
	for (size_t i = 0; i < ui.ResultList->count(); ++i) {
		auto pathItem = ui.ResultList->item(i);
		searchResults.append(pathItem->text());
	}
	state["SearchResults"] = searchResults;

	return state;
}

void FileSearch::LoadState(const QJsonObject& state)
{
	HX_JSON_SCOPE(state,
		HX_DESERIALIZE(ui.PathField, setText, toString);
	    HX_DESERIALIZE(ui.SearchPatternField, setText, toString);
	);

	auto searchResults = state["SearchResults"].toArray();
	for (auto searchResultJson : searchResults) {
		ui.ResultList->addItem(searchResultJson.toString());
	}
}

void FileSearch::OnBrowsePress()
{
	auto dir = QFileDialog::getExistingDirectory(this, "Select base directory for search");
	if (dir.isEmpty()) {
		return;
	}

	ui.PathField->setText(dir);
	emit Modified(this);
}

void FileSearch::OnSearchPress()
{
	ui.ResultList->clear();

	auto baseDirPath = ui.PathField->text();
	auto searchPattern = ui.SearchPatternField->text();
	FindFiles(baseDirPath, searchPattern);
	emit Modified(this);
}

void FileSearch::OnStopPress()
{
	stop = true;
}

void FileSearch::OnFileFound(std::filesystem::path foundPath)
{
	ui.ResultList->addItem(QString::fromStdString(foundPath.string()));
	emit Modified(this);
}

void FileSearch::OnSearchFinished()
{
	ui.SearchButton->setEnabled(true);
	ui.WalkedFileslabel->setVisible(false);
	ui.StopButton->setVisible(false);
	stop = false;
}

void FileSearch::OnFilesWalked(size_t count)
{
	ui.WalkedFileslabel->setText(QString("[walked %1 files]").arg(count));
}

void FileSearch::OnSearchResultDoubleClicked(QListWidgetItem* item)
{
	auto pathStr = item->text().toStdWString();

	// TODO: ...and this too, obviously
	if (auto idList = ILCreateFromPath(pathStr.data())) {
		SHOpenFolderAndSelectItems(idList, 0, nullptr, 0);
		ILFree(idList);
	}
}

void FileSearch::FindFiles(
	const QString& baseDirPath,
	const QString& searchPattern)
{
	if (searchThread.joinable()) {
		searchThread.join();
	}

	ui.SearchButton->setEnabled(false);
	ui.WalkedFileslabel->setVisible(true);
	ui.StopButton->setVisible(true);

	searchThread = std::thread([=] {
		std::vector<std::filesystem::path> files;
		std::filesystem::recursive_directory_iterator dirIt(baseDirPath.toStdString());
		auto searchRegexStr = QRegularExpression::wildcardToRegularExpression(
			searchPattern, QRegularExpression::UnanchoredWildcardConversion);
		QRegularExpression searchRegex(searchRegexStr, QRegularExpression::CaseInsensitiveOption);
		size_t walkedFiles{};
		for (auto& dirEntry : dirIt) {
			if (stop) {
				break;
			}

			if (dirEntry.is_directory()) {
				continue;
			}

			auto path = dirEntry.path();
			path.make_preferred();
			if (searchRegex.match(path.filename().c_str()).hasMatch()) {
				emit FileFound(path);
			}

			emit FilesWalked(++walkedFiles);
		}

		emit SearchFinished();
	});
}
