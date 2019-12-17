#include "ConsoleRemove.h"
#include "ConsoleCreate.h"
#include "ConsoleExtract.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace std;
using namespace Archive;

void ConsoleRemove::RemoveCommand(const ConsoleArgs& consoleArgs)
{
	string archiveFilename = GetArchiveName(consoleArgs);
	vector<string> filesToRemove = GetFilesToModify(consoleArgs);
	
	if (!consoleArgs.consoleSettings.quiet) {
		OutputInitialAddMessage(archiveFilename, filesToRemove.size());
	}

	unique_ptr<ArchiveFile> archive = ConsoleHelper::OpenArchive(archiveFilename);

	CheckFilesAvailableToRemove(*archive, filesToRemove, consoleArgs.consoleSettings.quiet);

	const vector<string> archiveInternalFilenames = RemoveMatchingFilenames(*archive, filesToRemove);

	ExtractFilesFromOriginalArchive(*archive, archiveInternalFilenames);

	archive.reset();

	vector<string> filenames = XFile::DirFiles(tempDirectory);

	for (auto& filename : filenames) {
		filename = XFile::Append(tempDirectory, filename);
	}

	CreateModifiedArchive(archiveFilename, filenames, consoleArgs.consoleSettings.quiet);
}

void ConsoleRemove::OutputInitialAddMessage(const string& archiveFilename, std::size_t fileCountToRemove)
{
	cout << "Attempting to remove " << fileCountToRemove << " file(s) from the archive " << archiveFilename << endl;
	cout << ConsoleHelper::dashedLine << endl;
}

vector<string> ConsoleRemove::RemoveMatchingFilenames(ArchiveFile& archive, const vector<string>& filesToRemove)
{
	vector<string> internalFilenames;

	for (std::size_t i = 0; i < archive.GetCount(); ++i) {
		internalFilenames.push_back(archive.GetName(i));
	}

	return StringHelper::RemoveStrings(internalFilenames, filesToRemove);
}

void ConsoleRemove::ThrowUnfoundFileDuringRemoveException(const vector<string>& unfoundFilenames)
{
	string exceptionString("The Following filename(s) were not found in the archive:");

	for (std::size_t i = 0; i < unfoundFilenames.size(); ++i)
	{
		exceptionString += " " + unfoundFilenames[i];

		if (i < unfoundFilenames.size() - 1) {
			exceptionString += ",";
		}
	}

	exceptionString += ".";

	throw runtime_error(exceptionString);
}

void ConsoleRemove::CheckFilesAvailableToRemove(ArchiveFile& archive, const vector<string>& filesToRemove, bool quiet)
{
	vector<string> internalFilenames;

	for (std::size_t i = 0; i < archive.GetCount(); ++i) {
		internalFilenames.push_back(archive.GetName(i));
	}

	// Unfound filenames are filenames requested for removal that do not exist in the archive.
	vector<string> unfoundFilenames = StringHelper::RemoveStrings(filesToRemove, internalFilenames);

	if (unfoundFilenames.size() > 0) {
		ThrowUnfoundFileDuringRemoveException(unfoundFilenames);
	}
}

// The REMOVE command actually extracts the contents of an archive, deletes the archive, and then recreates the archive.
void ConsoleRemove::ExtractFilesFromOriginalArchive(ArchiveFile& archive, const vector<string> internalFilenames)
{
	for (const auto& internalFilename : internalFilenames)
	{
		std::size_t index = archive.GetIndex(internalFilename);

		string pathToExtractTo = XFile::AppendSubDirectory(internalFilename, tempDirectory);

		try {
			archive.ExtractFile(index, pathToExtractTo);
		}
		catch (const std::exception& e) {
			throw runtime_error("Unable to extract file " + internalFilename + " from original archive. Operation Aborted. Internal Error Message: " + e.what());
		}
	}
}
