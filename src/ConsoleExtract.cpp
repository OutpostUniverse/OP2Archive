#include "ConsoleExtract.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <stdexcept>
#include <memory>

using namespace std;
using namespace Archive;

void ConsoleExtract::ExtractCommand(const ConsoleArgs& consoleArgs)
{
	ConsoleHelper::CheckIfPathsEmpty(consoleArgs.paths);

	string pathToExtractFrom = consoleArgs.paths[0];

	if (ConsoleHelper::IsArchiveExtension(pathToExtractFrom))
	{
		vector<string> filesToExtract(consoleArgs.paths.begin() + 1, consoleArgs.paths.end());
		ExtractFromArchive(pathToExtractFrom, filesToExtract, consoleArgs.consoleSettings);
	}
	else if (XFile::IsDirectory(pathToExtractFrom)) {
		ExtractFromDirectory(pathToExtractFrom, consoleArgs.consoleSettings);
	}
	else {
		throw runtime_error("You must specify either source archive file (.vol|.clm) or directory to extract from.");
	}
}

void ConsoleExtract::ExtractFromDirectory(const string& directory, const ConsoleSettings& consoleSettings)
{
	vector<string> archiveFilenames = ConsoleHelper::GetArchiveFilenames(directory);

	for (const auto& archiveFilename : archiveFilenames)
	{
		const string archivePath = XFile::Append(directory, archiveFilename);
		unique_ptr<ArchiveFile> archive = ConsoleHelper::OpenArchive(archivePath);
		ExtractAllFiles(*archive, consoleSettings);
	}
}

void ConsoleExtract::ExtractFromArchive(const string& archiveFilename, const vector<string>& filesToExtract, const ConsoleSettings& consoleSettings)
{
	unique_ptr<ArchiveFile> archiveFile = ConsoleHelper::OpenArchive(archiveFilename);

	// If no files provided, extract entire contents of archive.
	if (filesToExtract.size() == 0) {
		ExtractAllFiles(*archiveFile, consoleSettings);
		return;
	}

	// If specific files provided, only extract provided files.
	for (const auto& fileToExtract : filesToExtract) {
		ExtractSpecificFile(*archiveFile, fileToExtract, consoleSettings);
	}
}

void ConsoleExtract::ExtractAllFiles(ArchiveFile& archiveFile, const ConsoleSettings& consoleSettings)
{
	if (!consoleSettings.quiet) {
		cout << "Extracting all " << archiveFile.GetCount() << " file(s) from archive " << archiveFile.GetVolumeFilename() << "." << endl;
	}

	for (std::size_t i = 0; i < archiveFile.GetCount(); ++i) {
		ExtractSpecificFile(archiveFile, archiveFile.GetName(i), consoleSettings);
	}

	if (!consoleSettings.quiet) {
		cout << "Extraction Finished." << endl;
	}
}

void ConsoleExtract::ExtractSpecificFile(ArchiveFile& archiveFile, const string& filenameToExtract, const ConsoleSettings& consoleSettings)
{
	if (!XFile::PathExists(consoleSettings.destDirectory) || !XFile::IsDirectory(consoleSettings.destDirectory)) {
		XFile::NewDirectory(consoleSettings.destDirectory);
	}

	string destPath = XFile::ReplaceFilename(consoleSettings.destDirectory, filenameToExtract);

	if (!consoleSettings.overwrite) {
		if (CheckIfFileExists(destPath, consoleSettings.quiet)) {
			return;
		}
	}

	try {
		archiveFile.ExtractFile(filenameToExtract, destPath);
		if (!consoleSettings.quiet) {
			cout << filenameToExtract << " extracted." << endl;
		}
	}
	catch (const std::exception& e)
	{
		if (!consoleSettings.quiet) {
			cerr << e.what() << endl;
		}
	}
}

bool ConsoleExtract::CheckIfFileExists(const string& path, bool quiet)
{
	if (XFile::PathExists(path))
	{
		if (!quiet) {
			cerr << "A file with the same name already exists at " + path + "." << endl;
		}

		return true;
	}

	return false;
}
