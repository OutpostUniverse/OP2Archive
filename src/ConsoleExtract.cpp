#include "ConsoleExtract.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <stdexcept>
#include <memory>

using namespace std;
using namespace Archives;

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
		string archivePath = XFile::AppendSubDirectory(archiveFilename, directory);
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
		cout << "Extracting all " << archiveFile.GetNumberOfPackedFiles() << " file(s) from archive " << archiveFile.GetVolumeFilename() << "." << endl;
	}

	for (int i = 0; i < archiveFile.GetNumberOfPackedFiles(); ++i) {
		ExtractSpecificFile(archiveFile, archiveFile.GetInternalFilename(i), consoleSettings);
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
	int archiveFileIndex = archiveFile.GetInternalFileIndex(filenameToExtract);

	if (archiveFileIndex == -1) {
		throw runtime_error("Provided filename does not exist in the archive: " + filenameToExtract + ".");
	}

	if (!consoleSettings.overwrite) {
		if (CheckIfFileExists(destPath, consoleSettings.quiet)) {
			return;
		}
	}

	try {
		archiveFile.ExtractFile(archiveFileIndex, destPath);
		if (!consoleSettings.quiet) {
			cout << filenameToExtract << " extracted." << endl;
		}
	}
	catch (const std::exception&)
	{
		if (!consoleSettings.quiet) {
			cerr << "Error extracting " << filenameToExtract << endl;
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
