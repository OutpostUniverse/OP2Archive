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

	for (string archiveFilename : archiveFilenames)
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
	for (size_t i = 0; i < filesToExtract.size(); ++i) {
		ExtractSpecificFile(*archiveFile, filesToExtract[i], consoleSettings);
	}
}

void ConsoleExtract::ExtractAllFiles(ArchiveFile& archiveFile, const ConsoleSettings& consoleSettings)
{
	if (!consoleSettings.quiet) {
		cout << "Extracting all " << archiveFile.GetNumberOfPackedFiles() << " file(s) from archive " << archiveFile.GetVolumeFileName() << "." << endl;
	}

	for (int i = 0; i < archiveFile.GetNumberOfPackedFiles(); ++i) {
		ExtractSpecificFile(archiveFile, archiveFile.GetInternalFileName(i), consoleSettings);
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

	string destPath = XFile::ReplaceFilename(consoleSettings.destDirectory, filenameToExtract).c_str();
	int archiveFileIndex = archiveFile.GetInternalFileIndex(filenameToExtract.c_str());

	if (archiveFileIndex == -1) {
		throw runtime_error("Provided filename does not exist in the archive: " + filenameToExtract + ".");
	}

	if (!consoleSettings.overwrite) {
		if (CheckIfFileExists(destPath, consoleSettings.quiet)) {
			return;
		}
	}

	bool success = archiveFile.ExtractFile(archiveFileIndex, destPath.c_str());

	if (!consoleSettings.quiet) {
		OutputExtractionMessage(success, filenameToExtract);
	}
}

bool ConsoleExtract::CheckIfFileExists(string path, bool quiet)
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

void ConsoleExtract::OutputExtractionMessage(bool success, string filename)
{
	if (success) {
		cout << filename << " extracted." << endl;
	}
	else {
		cerr << "Error extracting " << filename << endl;
	}
}
